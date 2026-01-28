//
// Copyright (C) 2018 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include "Fsr.h"
#include "FsrProtocolDissector.h"
#include "FsrLSU_m.h"

#include "inet/common/packet/dissector/ProtocolDissectorRegistry.h"

namespace inet {

Register_Protocol_Dissector(&fsrProtocol, FsrProtocolDissector);

void FsrProtocolDissector::dissect(Packet *packet, const Protocol *protocol, ICallback& callback) const
{
    // TODO: This is just a stub
    auto header = packet->popAtFront<LSUPacket>();
    callback.startProtocolDataUnit(&fsrProtocol);
    callback.visitChunk(header, &fsrProtocol);
    callback.endProtocolDataUnit(&fsrProtocol);
}

} // namespace inet

