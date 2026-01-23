import inet.common.INETDefs;
import inet.common.packet.chunk.Chunk;
import inet.networklayer.contract.ipv4.Ipv4Address;

namespace inet;

cplusplus {{
using namespace inet;
}}

// This LinkInfo class is used in the implementation, but not in the protocol definition.
// It is not part of the LSU.
class LinkInfo {
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

    Ipv4Address links[]; // Addresses of directly connected neighbors. Cost is 1.
    simtime_t timestamp = 0; // Time at which link state was generated at source node (node that the address belongs to)
};

class LSUPacket extends FieldsChunk
{
    Ipv4Address srcAddress;
    LinkState linkStates[];
}

struct ScopePeriod {
    unsigned int scope;
    simtime_t period;
}

class FsrScopesParam extends cOwnedObject {
    ScopePeriod scopes[];
}
