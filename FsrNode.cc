#include <memory.h>
#include <stdlib.h>

#include <map>
#include <string>

#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/networklayer/common/HopLimitTag_m.h"
#include "inet/networklayer/common/L3AddressTag_m.h"

#include "FsrNode.h"
#include <queue>

namespace inet {
namespace fsrv2 {

Define_Module(FsrNode);

FsrNode::FsrNode()
{
}

FsrNode::~FsrNode()
{
    cancelAndDelete(startupTimer);
}

void FsrNode::initialize(int stage)
{
    RoutingProtocolBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        EV_INFO << "Initializing FSR node\n";

        host = getContainingNode(this);
        ift.reference(this, "interfaceTableModule", true);
        rt.reference(this, "routingTableModule", true);

        // Can't get the interface id here because the table is not yet initialized

        startupTimer = new cMessage("FSR-startup");

        linkTimeout = par("linkTimeout").doubleValue();

        maxScope = 1;
        ScopesParam *scopesPar = check_and_cast<ScopesParam *>(par("scopes").objectValue());
        for (int i = 0; i < scopesPar->getScopesArraySize(); i++) {
            scopes.push_back(scopesPar->getScopes(i));

            if (scopesPar->getScopes(i).scope > maxScope) {
                maxScope = scopesPar->getScopes(i).scope;
            }
        }
    }
    else if (stage == INITSTAGE_ROUTING_PROTOCOLS) { // interfaces and static routes are already initialized
        EV_INFO << "Initializing FSR protocol\n";
        registerProtocol(Protocol::fsr, gate("ipOut"), gate("ipIn"));
    }
}

void FsrNode::handleMessageWhenUp(cMessage *msg)
{
    if (msg == startupTimer) {
        EV_DETAIL << "FSR starting up\n";
        handleStartUpTimer();
    }
    else {
        if (msg->isSelfMessage()) {
            if (strcmp(msg->getName(), "FsrScopeUpdate") != 0) {
                EV_WARN << "Unexpected FSR self message:" << msg->getName() << "\n";
                return;
            }

            EV_DETAIL << "FSR scope update\n";

            // Handle timer msg, check its scope and send LSU accordingly
            ScopePeriod *periodicScope = reinterpret_cast<ScopePeriod *>(msg->getContextPointer());

            handleScopeUpdate(periodicScope);

            // Reschedule the timer
            scheduleAfter(periodicScope->period, msg);
        }
        else {
            EV_DETAIL << "FSR Link State Update\n";

            // This is an LSU from another node. Update your topology table.
            Packet *pk = check_and_cast<Packet *>(msg);
            auto protocol = pk->getTag<PacketProtocolTag>()->getProtocol();
            if (protocol == &Protocol::icmpv4) {
                EV_ERROR << "ICMP error received -- discarding\n";
                delete msg;
            }
            else if (protocol == &Protocol::fsr) {
                const auto& lsu = pk->peekAtFront<LSUPacket>();

                // Process the LSU and update the topology table, then update routing table if needed

                // First, update our neighbor list with this incoming node
                Ipv4Address srcAddr = lsu->getSrcAddress();
                EV_DETAIL << "Received LSU from " << srcAddr << "\n";
                if (neighborList.find(srcAddr) == neighborList.end()) {
                    // This is a new neighbor
                    NeighborInfo neighborInfo;
                    neighborInfo.link.setAddress(srcAddr);
                    neighborInfo.lastLsuTime = simTime();
                    neighborList[srcAddr] = neighborInfo;

                    // Add a link to the neighbor in our own link state
                    LinkInfo linkInfo;
                    linkInfo.setAddress(srcAddr);
                    linkInfo.setCost(1); // Cost to neighbor is 1 hop

                    topologyTable[routerId].setLinksArraySize(topologyTable[routerId].getLinksArraySize() + 1);
                    topologyTable[routerId].setLinks(topologyTable[routerId].getLinksArraySize() - 1, linkInfo);
                    topologyTable[routerId].setTimestamp(simTime());

                    // Technically we can also add a link to us to the neighbor's LSU, but it should not affect the routing algorithm locally

                    EV_DETAIL << "New neighbor added: " << srcAddr << "\n";
                }
                else {
                    // This is an existing neighbor, just update the last LSU time
                    neighborList[srcAddr].lastLsuTime = simTime();
                }

                // Now, update the topology table with the new link states
                for (int i = 0; i < lsu->getLinkStatesArraySize(); i++) {
                    LinkState linkState = lsu->getLinkStates(i);
                    Ipv4Address nodeAddr = linkState.getRouterId();

                    EV_DETAIL << "Processing link state for node: " << nodeAddr << " with time: " << linkState.getTimestamp() << "\n";

                    if (topologyTable.find(nodeAddr) != topologyTable.end())
                        EV_DETAIL << "Current link state for node: " << nodeAddr << " is: " << topologyTable[nodeAddr].getTimestamp() << "\n";

                    // If the node is not in the topology table, or if the link state is newer than the one we have, update it
                    if (topologyTable.find(nodeAddr) == topologyTable.end()
                        || linkState.getTimestamp() > topologyTable[nodeAddr].getTimestamp()) {
                        topologyTable[nodeAddr] = linkState;
                        EV_DETAIL << "Updated link state for node: " << nodeAddr << "\n";
                    }
                }

                delete msg; // Delete the packet after processing
                
                EV_DETAIL << "FSR Routing now...\n";

                // Finally, run the routing algorithm to update the routing and distance tables
                calcShortestPath();
            } else {
                EV_WARN << "Unexpected FSR protocol: " << protocol;
            }
        }
    }
}

void FsrNode::neighborChecks() {
    // First, check if any of our neighbor links are no longer valid
    bool neighborRemoved = false;
    for (auto it = neighborList.begin(); it != neighborList.end(); ) {
        if (simTime() - it->second.lastLsuTime >= linkTimeout) {
            // Remove neighbor from the list
            it = neighborList.erase(it);
            neighborRemoved = true;

            EV_DETAIL << "Removed neighbor: " << it->first << " due to timeout\n";
        }
        else {
            ++it; // C++ shenanigans (we're incrementing the iterator here so we can erase stuff)
        }
    }

    // If any neighbors were removed, we need to update the topology table
    if (neighborRemoved) {
        // Update timestamp for this router
        topologyTable[routerId].setTimestamp(simTime());
        topologyTable[routerId].setLinksArraySize(neighborList.size());
        
        int i = 0;
        for (const auto& neighbor : neighborList) {
            topologyTable[routerId].setLinks(i, neighbor.second.link);
            i++;
        }

        // We should rerun dijkstra and update the distance table. the scopes might be different because hops are different now.
        // If we don't, then the LSU we send will be outdated.
        calcShortestPath();
    }
}

std::vector<Ipv4Address> FsrNode::getAddressesOfScope(unsigned int scope) {
    EV_DETAIL << "Getting addresses of scope: " << scope << "\n";

    // Get all nodes in the scope we're targeting
    std::vector<Ipv4Address> addresses;
    for (auto link : distanceTable) {
        EV_DETAIL << "Checking link: " << link.getAddress() << " with cost: " << link.getCost() << "\n";

        // This is "distance in hops", not cost
        if (link.getCost() == scope
            || (scope == maxScope && link.getCost() >= maxScope)) {
            addresses.push_back(link.getAddress());
            EV_DETAIL << "Added address: " << link.getAddress() << " to scope " << scope << "\n";
        }
    }

    if (scope == 1) {
        // If the scope is 1, we should also add our own router ID
        addresses.push_back(routerId);
        EV_DETAIL << "Added own router ID to scope 1 addresses: " << routerId << "\n";
    }

    return addresses;
}

void FsrNode::sendPacket(inet::Ptr<LSUPacket> lsuPacket) {
    Packet *pk = new Packet();
    pk->insertAtBack(lsuPacket);

    pk->addTagIfAbsent<PacketProtocolTag>()->setProtocol(&Protocol::fsr);

    pk->addTagIfAbsent<InterfaceReq>()->setInterfaceId(outputIfIndex);

    // Broadcast so that all direct neighbors can receive
    pk->addTagIfAbsent<L3AddressReq>()->setDestAddress(Ipv4Address::ALLONES_ADDRESS);

    // TTL (max hop count) set to 1 because we do not want the packet to propagate past neighbors
    pk->addTagIfAbsent<HopLimitReq>()->setHopLimit(1);

    pk->addTagIfAbsent<DispatchProtocolReq>()->setProtocol(&Protocol::ipv4);
    send(pk, "ipOut");
}

void FsrNode::handleScopeUpdate(ScopePeriod *periodicScope) {
    neighborChecks();

    auto addresses = getAddressesOfScope(periodicScope->scope);

    const auto& lsuPacket = makeShared<LSUPacket>();

    // Source address is this router
    lsuPacket->setSrcAddress(routerId);
    B packetSize = B(sizeof(Ipv4Address));

    // Add link states
    lsuPacket->setLinkStatesArraySize(addresses.size());

    EV_DETAIL << "Adding link states for addresses: " << "\n";
    for (int i = 0; i < addresses.size(); i++) {
        // Print links of link state
        EV_DETAIL << "Link state for address " << addresses[i] << "\n";
        for (int j = 0; j < topologyTable[addresses[i]].getLinksArraySize(); j++) {
            EV_DETAIL << "  Link " << j << ": " << topologyTable[addresses[i]].getLinks(j).getAddress()
                      << " with cost: " << topologyTable[addresses[i]].getLinks(j).getCost() << "\n";
        }

        lsuPacket->setLinkStates(i, topologyTable[addresses[i]]);
        packetSize += B(sizeof(Ipv4Address)) + B(sizeof(simtime_t)) + B(topologyTable[addresses[i]].getLinksArraySize() * sizeof(LinkInfo));
    }

    lsuPacket->setChunkLength(packetSize);

    sendPacket(lsuPacket);
}

void FsrNode::calcShortestPath() {
    std::priority_queue<LinkInfo, std::vector<LinkInfo>, std::greater<LinkInfo>> pq;

    EV_DETAIL << "Calculating shortest path for router: " << routerId << "\n";

    // Distance table
    std::map<Ipv4Address, unsigned long> dist;
    std::map<Ipv4Address, Ipv4Address> nextHop;
    for (auto it : topologyTable) {
        dist[it.first] = ULONG_MAX;
        nextHop[it.first] = Ipv4Address::UNSPECIFIED_ADDRESS;

        EV_DETAIL << "Router: " << it.first << " initialized with distance: " << dist[it.first] << "\n";
    }

    // Insert source itself in priority queue and initialize
    // its distance as 0.
    pq.push(LinkInfo {routerId, 0});
    dist[routerId] = 0;

    // Looping till priority queue becomes empty (or all
    // distances are not finalized) 
    while (!pq.empty()){
        // The first vertex in pair is the minimum distance
        // vertex, extract it from priority queue.
        Ipv4Address u = pq.top().getAddress();
        pq.pop();

        // Get all adjacent of u.
        for (int i = 0; i < topologyTable[u].getLinksArraySize(); i++){
            // Get vertex label and weight of current
            // adjacent of u.
            Ipv4Address v = topologyTable[u].getLinks(i).getAddress();
            int weight = topologyTable[u].getLinks(i).getCost();

            EV_DETAIL << "Checking link from " << u << " to " << v << " with weight: " << weight << "\n";

            // If there is shorter path to v through u.
            if (dist[v] > dist[u] + weight)
            {
                // Updating distance of v and next hop
                dist[v] = dist[u] + weight;
                nextHop[v] = (u == routerId) ? v : nextHop[u];
                pq.push(LinkInfo {v, dist[v]});

                EV_DETAIL << "Updated distance for " << v << " to " << dist[v] << " via " << nextHop[v] << "\n";
            }
        }
    }

    // Update router's distance table
    // FIXME: This is only correct if the cost of all links is exactly 1
    // That's because "dist" contains costs, but "distanceTable" should contain hop count
    distanceTable.clear();
    for (auto it : dist) {
        distanceTable.push_back(LinkInfo {it.first, it.second});
        EV_DETAIL << "Distance table updated: " << it.first << " with cost: " << it.second << "\n";
    }

    EV_DETAIL << "Current routing table:\n";
    rt->printRoutingTable();

    // for (int32_t i = 0; i < rt->getNumRoutes(); i++) {
    //     Ipv4Route *route = rt->getRoute(i);
    //     if (route) {
    //         EV_DETAIL << "Route " << i << ": "
    //                   << "Destination: " << route->getDestination()
    //                   << ", Next Hop: " << route->getNextHopAsGeneric()
    //                   << ", Interface: " << route->getInterface()->getInterfaceName()
    //                   << ", Metric: " << route->getMetric() << "\n";
    //     }
    // }

    // Update routing table
    for (auto it : nextHop) {
        if (it.first == routerId) {
            // Skip self routes
            continue;
        }

        IRoute *route = rt->findBestMatchingRoute(it.first);

        if (route == nullptr) {
            route = rt->createRoute();
            route->setDestination(it.first);
            route->setPrefixLength(32); // ??? Assuming /32 for host routes
            route->setInterface(ift->getInterfaceById(outputIfIndex));

            rt->addRoute(route);
            EV_DETAIL << "Created new route for " << it.first << ": " << route << "\n";
        }

        route->setNextHop(it.second);
        EV_DETAIL << "Setting next hop for " << it.first << " to " << it.second << "\n";
    }

    // Print the routing table
    EV_DETAIL << "Updated routing table:\n";
    rt->printRoutingTable();
    // for (int32_t i = 0; i < rt->getNumRoutes(); i++) {
    //     Ipv4Route *route = rt->getRoute(i);
    //     if (route) {
    //         EV_DETAIL << "Route " << i << ": "
    //                   << "Destination: " << route->getDestination()
    //                   << ", Next Hop: " << route->getNextHopAsGeneric()
    //                   << ", Interface: " << route->getInterface()->getInterfaceName()
    //                   << ", Metric: " << route->getMetric() << "\n";
    //     }
    // }
}

void FsrNode::handleStartUpTimer()
{
    // Get router ID from the referenced routing table
    routerId = rt->getRouterId();
    EV_INFO << "Initializing FSR node with router ID: " << routerId << "\n";

    if (outputIfIndex == -1) {
        for (int i = 0; i < ift->getNumInterfaces(); i++) {
            NetworkInterface *iface = ift->getInterface(i);
            if (strcmp(iface->getInterfaceName(), "wlan0") == 0) {
                outputIfIndex = iface->getInterfaceId();
                EV_INFO << "Found wlan0 interface with ID " << outputIfIndex << "\n";
                break;
            }
        }
    }

    // Initialize this router's link state in the table
    LinkState ls = LinkState();
    ls.setRouterId(routerId);
    ls.setTimestamp(simTime());

    topologyTable[routerId] = ls;
    distanceTable.push_back(LinkInfo { routerId, 0 });

    subscribe();

    // Schedule first self message
    for (int i = 0; i < scopes.size(); i++) {
        ScopePeriod *scope = &scopes[i];
        cMessage *scopeMsg = new cMessage("FsrScopeUpdate");
        scopeMsg->setContextPointer(scope);

        scheduleAfter(scope->period, scopeMsg);
    }
}

void FsrNode::subscribe()
{
    host->subscribe(interfaceCreatedSignal, this);
    host->subscribe(interfaceDeletedSignal, this);
    host->subscribe(interfaceStateChangedSignal, this);
}

void FsrNode::unsubscribe()
{
    host->unsubscribe(interfaceCreatedSignal, this);
    host->unsubscribe(interfaceDeletedSignal, this);
    host->unsubscribe(interfaceStateChangedSignal, this);
}

/**
 * Listen on interface changes and update private data structures.
 */
void FsrNode::receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject *details)
{
    Enter_Method("%s", cComponent::getSignalName(signalID));

    const NetworkInterface *ie;
    const NetworkInterfaceChangeDetails *change;

    if (signalID == interfaceCreatedSignal) {
        // configure interface for RIP
        ie = check_and_cast<const NetworkInterface *>(obj);
        if (ie->isMulticast() && !ie->isLoopback()) {
            // TODO
        }
    }
    else if (signalID == interfaceDeletedSignal) {
        ie = check_and_cast<const NetworkInterface *>(obj);
        // TODO
    }
    else if (signalID == interfaceStateChangedSignal) {
        change = check_and_cast<const NetworkInterfaceChangeDetails *>(obj);
        auto fieldId = change->getFieldId();
        if (fieldId == NetworkInterface::F_STATE || fieldId == NetworkInterface::F_CARRIER) {
            ie = change->getNetworkInterface();
            if (!ie->isUp())
                handleInterfaceDown(ie);
            else {
                // interface went back online. Do nothing!
                // Wait for Hello messages to establish adjacency.
            }
        }
    }
    else
        throw cRuntimeError("Unexpected signal: %s", getSignalName(signalID));
}

void FsrNode::handleStartOperation(LifecycleOperation *operation)
{
    simtime_t startupTime = par("startupTime");
    if (startupTime <= simTime()) {
        scheduleAfter(SimTime(1, SIMTIME_MS), startupTimer);
    }
    else
        scheduleAfter(startupTime, startupTimer);
}

void FsrNode::handleStopOperation(LifecycleOperation *operation)
{
    cancelEvent(startupTimer);
    unsubscribe();
}

void FsrNode::handleCrashOperation(LifecycleOperation *operation)
{
    cancelEvent(startupTimer);
    unsubscribe();
}

void FsrNode::handleInterfaceDown(const NetworkInterface *ie)
{
    EV_DETAIL << "DEBUG: interface " << ie->getInterfaceId() << " went down. \n";

    // Step 1: delete all direct-routes connected to this interface

    // ... from Ipv4 table
    for (int32_t i = 0; i < rt->getNumRoutes(); i++) {
        Ipv4Route *route = rt->getRoute(i);
        if (route && route->getInterface() == ie && route->getNextHopAsGeneric().isUnspecified()) {
            EV_DETAIL << "DEBUG: removing route from Ipv4 routing table: " << route << "\n";
            rt->deleteRoute(route);
            i--;
        }
    }
}

} // namespace fsr
} // namespace inet

