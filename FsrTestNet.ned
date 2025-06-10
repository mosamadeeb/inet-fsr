package inet.routing.fsrv2;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.ManetRouter;
import inet.physicallayer.wireless.common.contract.packetlevel.IRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;

network FsrTestNet
{
    parameters:
        int n @prompt("Number of intermediate nodes") = default(10);

        // Limit MTU to enable fragmentation (AckingMac mtu is 4kB by default)
        *.mtu = 1500B;

        // // https://stackoverflow.com/a/49667047
        // volatile int posX = intuniform (100,400);
        // volatile int posY = intuniform (50,600);

        @display("bgb=650,600;bgg=100,1,grey95");
        @figure[title](type=label; pos=0,-1; anchor=sw; color=darkblue);

        @figure[rcvdPkText](type=indicatorText; pos=380,20; anchor=w; font=,18; textFormat="packets received: %g"; initialValue=0);
        @statistic[packetReceived](source=hostB.app[0].packetReceived; record=figure(count); targetFigure=rcvdPkText);

    submodules:
        visualizer: <default(firstAvailableOrEmpty("IntegratedCanvasVisualizer"))> like IIntegratedVisualizer if typename != "" {
            @display("p=580,125");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=580,200");
        }
        radioMedium: <default("UnitDiskRadioMedium")> like IRadioMedium {
            @display("p=580,275");
        }
        hostA: ManetRouter {
            @display("p=50,325");
            *.startupTime = 0.010s;
        }
        hostB: ManetRouter {
            @display("p=450,325");
            *.startupTime = 0.020s;
        }

        hostR[n]: ManetRouter {
            // Different startup times to avoid collisions
            *.startupTime = 0.030s + (0.010s * index);
        }
}

