//
// Copyright (C) 2018 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include "FsrProtocolDissector.h"
#include "packet_m.h"

#include "inet/common/packet/dissector/ProtocolDissectorRegistry.h"

namespace inet {
namespace fsrv2 {

Register_Protocol_Dissector(&Protocol::fsr, FsrProtocolDissector);

void FsrProtocolDissector::dissect(Packet *packet, const Protocol *protocol, ICallback& callback) const
{
    auto header = packet->popAtFront<LSUPacket>();
    callback.startProtocolDataUnit(&Protocol::fsr);
    callback.visitChunk(header, &Protocol::fsr);
    callback.endProtocolDataUnit(&Protocol::fsr);
}

} // namespace fsrv2
} // namespace inet

