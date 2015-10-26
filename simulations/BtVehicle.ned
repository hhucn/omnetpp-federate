//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package federate.simulations;

import inet.base.NotificationBoard;
import inet.transport.udp.UDP;
import inet.transport.tcp.TCP;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv4.RoutingTable;
import inet.nodes.inet.NetworkLayer;
import inet.linklayer.ieee80211.Ieee80211NewNicAdhoc;

import federate.msg.BgTrafficApp;
import federate.mob.VSimRTIMobility;

//
// One vehicle defined as compound module.
//
module BtVehicle
{
    parameters:
        @node;
        @display("bgb=458,464");
    gates:
        input radioIn @directIn;
    submodules:
        // Base for module interaction
        notificationBoard: NotificationBoard {
            parameters:
                @display("p=60,238");
        }
        // Application layer
        bgTrafficApp: BgTrafficApp {
            @display("p=345,48");
        }
        // Transport layer
        udp: UDP {
            @display("p=345,145");
        }
        // Network layer
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=60,48");
        }
        routingTable: RoutingTable {
            parameters:
                IPForward = true;
                routerId = "";
                routingFile = "";
                @display("p=59,145");
        }
        networkLayer: NetworkLayer {
            parameters:
                proxyARP = false;
                @display("p=345,238");
            gates:
                ifIn[1];
                ifOut[1];
        }
        // MAC and PHY
        wlan: Ieee80211NewNicAdhoc {
            parameters:
                @display("p=345,333;q=queue;i=block/ifcard");
        }
        // Mobility
        mobility: VSimRTIMobility {
            @display("p=60,333");
        }
    connections allowunconnected:
        // Application <--> Transport
        bgTrafficApp.udpOut --> udp.appIn++;
        udp.appOut++ --> bgTrafficApp.udpIn;
        // Transport <--> Network
        udp.ipOut --> networkLayer.udpIn;
        networkLayer.udpOut --> udp.ipIn;
        // Network <--> MAC (and PHY)
        networkLayer.ifOut[0] --> wlan.uppergateIn;
        wlan.uppergateOut --> networkLayer.ifIn[0];
		// Lower layers <--> Radio channel
        radioIn --> wlan.radioIn;
}
