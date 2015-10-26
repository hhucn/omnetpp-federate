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

import federate.msg.VSimRTIUnreliableApp;
import federate.msg.VSimRTIReliableApp;
import federate.mob.VSimRTIMobility;

//
// One vehicle defined as compound module.
//
module Rsu
{
    parameters:
        @node;
        @display("bgb=284,404");
        int numRadios = 1;
    gates:
        input radioIn[numRadios] @directIn;
    submodules:
        // Base for module interaction
        notificationBoard: NotificationBoard {
            parameters:
                @display("p=60,238");
        }
        // Application layer
        vSimRTIUnreliableApp: VSimRTIUnreliableApp {
            @display("p=198,48");
        }
        // Transport layer
        udp: UDP {
            @display("p=198,145");
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
                @display("p=198,238");
            gates:
                ifIn[numRadios];
                ifOut[numRadios];
        }
        // MAC and PHY
        wlan[numRadios]: Ieee80211NewNicAdhoc {
            parameters:
                @display("p=198,333;q=queue;i=block/ifcard");
                OpMode = "p";
        }
        // Mobility
        mobility: VSimRTIMobility {
            @display("p=60,333");
        }
    connections allowunconnected:
        // Application <--> Transport
        vSimRTIUnreliableApp.udpOut --> udp.appIn++;
        udp.appOut++ --> vSimRTIUnreliableApp.udpIn;
        // Transport <--> Network
        udp.ipOut --> networkLayer.udpIn;
        networkLayer.udpOut --> udp.ipIn;
        // Network <--> MAC (and PHY)
        for i=0..numRadios-1 {
            networkLayer.ifOut[i] --> wlan[i].uppergateIn;
            wlan[i].uppergateOut --> networkLayer.ifIn[i];
        }
        // Lower layers <--> Radio channel
        for i=0..numRadios-1 {
            radioIn[i] --> wlan[i].radioIn;
        }
}
