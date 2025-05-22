import inet.common.INETDefs;
import inet.networklayer.contract.ipv4.Ipv4Address;

namespace inet::fsrv2;

cplusplus {{
using namespace inet;
}}


class LinkInfo {
    @packetData;
    Ipv4Address address;
    unsigned long cost = 1; // This also acts as distance in SPF algorithm
};

cplusplus(LinkInfo) {{
  public:
    LinkInfo(Ipv4Address address, unsigned long cost);
    bool operator>(const LinkInfo& other) const;
}}

cplusplus(cc) {{
LinkInfo::LinkInfo(Ipv4Address address, unsigned long cost) {
    this->address = address;
    this->cost = cost;
}

// Overload the comparison operator for the priority queue
bool LinkInfo::operator>(const LinkInfo& other) const {
    return cost > other.cost;
}
}}

class LinkState {
    @packetData;
    Ipv4Address routerId;

    LinkInfo links[];
    simtime_t timestamp = 0; // Time at which link state was generated at source node (node that the address belongs to)
};

packet LSUPacket
{
    Ipv4Address srcAddress;
    LinkState linkStates[];
}
