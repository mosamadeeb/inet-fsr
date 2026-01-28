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

        @display("bgb=1000,600;bgg=100,1,grey95");
        @figure[title](type=label; pos=0,-1; anchor=sw; color=darkblue);

        @figure[rcvdPkText](type=indicatorText; pos=380,20; anchor=w; font=,18; textFormat="packets received: %g"; initialValue=0);
        @statistic[packetReceived](source=hostB.app[0].packetReceived; record=figure(count); targetFigure=rcvdPkText);

    submodules:
        visualizer: <default(firstAvailableOrEmpty("IntegratedCanvasVisualizer"))> like IIntegratedVisualizer if typename != "" {
            @display("p=930,125");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=930,200");
        }
        radioMedium: <default("UnitDiskRadioMedium")> like IRadioMedium {
            @display("p=930,275");
        }
        hostA: ManetRouter {
            @display("p=100,325");
            *.startupTime = 0.010s;
        }
        hostB: ManetRouter {
            @display("p=800,325");
            *.startupTime = 0.020s;
        }

        hostR[n]: ManetRouter {
            // Different startup times to avoid collisions
            *.startupTime = 0.030s + (0.010s * index);
        }
}

