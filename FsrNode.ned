//
// Copyright (C) 2020 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


package inet.routing.fsrv2;

import inet.routing.contract.IManetRouting;


simple FsrNode like IManetRouting
{
    parameters:
        string interfaceTableModule;
        string routingTableModule;

        volatile double startupTime @unit(s) = default(0.010s); // delay before starting the router
        volatile double linkTimeout @unit(s) = default(2s); // delay before removing a neighbor

        object scopes = default(inet::fsrv2::ScopesParam {
            scopes: [
                { scope: 1, period: 500ms },
                { scope: 2, period: 750ms },
                { scope: 3, period: 1000ms }
            ]
        });

        @display("i=block/network2");
    gates:
        input ipIn @labels(Ipv4ControlInfo/up);
        output ipOut @labels(Ipv4ControlInfo/down);
}
