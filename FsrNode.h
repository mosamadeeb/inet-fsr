#ifndef __INET_FSRNODE_H
#define __INET_FSRNODE_H

#include <vector>
#include <map>

#include "inet/common/ModuleRefByPar.h"
#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/networklayer/ipv4/IIpv4RoutingTable.h"
#include "inet/routing/base/RoutingProtocolBase.h"
#include "inet/networklayer/contract/ipv4/Ipv4Address.h"

#include "packet_m.h"

namespace inet {

namespace fsrv2 {

const Protocol FSR_PROTOCOL("fsr", "FSR");

struct NeighborInfo {
    LinkInfo link;
    simtime_t lastLsuTime;
};

struct ScopePeriod {
    unsigned int scope;
    simtime_t period;
};

class INET_API FsrNode : public RoutingProtocolBase, protected cListener
{
  private:
    cModule *host = nullptr; // the host module that owns this module
    ModuleRefByPar<IIpv4RoutingTable> rt;
    ModuleRefByPar<IInterfaceTable> ift;
    Ipv4Address routerId;
    cMessage *startupTimer = nullptr; // timer for delayed startup

    // FSR Data Structures
    std::map<Ipv4Address, NeighborInfo> neighborList;
    std::map<Ipv4Address, LinkState> topologyTable;
    std::vector<LinkInfo> distanceTable; // Distance in hop counts
    // No nextHopTable because the routing table rt is used instead

    // Parameters
    simtime_t linkTimeout = SimTime(10, SIMTIME_S);
    std::vector<ScopePeriod> scopes;


  public:
    FsrNode();
    virtual ~FsrNode();

  protected:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject *details) override;
    virtual void subscribe();
    virtual void unsubscribe();

    void handleScopeUpdate(ScopePeriod *periodicScope);
    void calcShortestPath();

    // lifecycle
    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;

    void handleInterfaceDown(const NetworkInterface *ie);
};

} // namespace fsr

} // namespace inet

#endif

