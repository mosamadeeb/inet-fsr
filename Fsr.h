#ifndef __INET_FSR_H
#define __INET_FSR_H

#include <vector>
#include <map>

#include "inet/common/ModuleRefByPar.h"
#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/networklayer/ipv4/IIpv4RoutingTable.h"
#include "inet/routing/base/RoutingProtocolBase.h"
#include "inet/networklayer/contract/ipv4/Ipv4Address.h"

#include "FsrLSU_m.h"

static const inet::Protocol fsrProtocol("fsr", "FSR");

namespace inet {

struct NeighborInfo {
    Ipv4Address address;
    simtime_t lastLsuTime;
};

class INET_API Fsr : public RoutingProtocolBase, protected cListener
{
  private:
    cModule *host = nullptr; // the host module that owns this module
    ModuleRefByPar<IIpv4RoutingTable> rt;
    ModuleRefByPar<IInterfaceTable> ift;
    Ipv4Address routerId;
    int outputIfIndex = -1;
    cMessage *startupTimer = nullptr; // timer for delayed startup
    unsigned int maxScope;

    // Protocol fsrProtocol;

    // FSR Data Structures
    std::map<Ipv4Address, NeighborInfo> neighborList;
    std::map<Ipv4Address, LinkState> topologyTable;
    std::vector<LinkInfo> distanceTable; // Distance in hop counts
    // No nextHopTable because the routing table rt is used instead

    // Parameters
    simtime_t linkTimeout;
    std::vector<ScopePeriod> scopes;

  public:
    Fsr();
    virtual ~Fsr();

  protected:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject *details) override;
    virtual void subscribe();
    virtual void unsubscribe();

    void handleStartUpTimer();
    void handleScopeUpdate(ScopePeriod *periodicScope);
    void calcShortestPath();

    // lifecycle
    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;

    void handleInterfaceDown(const NetworkInterface *ie);

    void neighborChecks();
    std::vector<Ipv4Address> getAddressesOfScope(unsigned int scope);
    void sendPacket(inet::Ptr<LSUPacket> lsuPacket);
};

} // namespace inet

#endif
