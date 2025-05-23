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

        volatile double startupTime @unit(s) = default(0s); // delay before starting the router
        volatile double linkTimeout @unit(s) = default(10s); // delay before removing a neighbor

        object scopes = default(ScopesParam {
            scopes: [
                { scope: 1, period: 3s },
                { scope: 2, period: 6s },
                { scope: 3, period: 9s }
            ]
        });

        @display("i=block/network2");
    gates:
        input ipIn @labels(Ipv4ControlInfo/up);
        output ipOut @labels(Ipv4ControlInfo/down);
}
