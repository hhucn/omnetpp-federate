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

import inet.world.ChannelControlExtended;
import inet.networklayer.autorouting.FlatNetworkConfigurator;
import federate.mgmt.VSimRTIScenarioManager;

//
// Simulation setup for vsimrti controlled simulation.
//
network Simulation
{
    parameters:
        double playgroundSizeX;
        double playgroundSizeY;
        @display("bgb=375,197");
        int numChannels = 2;
    submodules:
        channelControl: ChannelControlExtended {
            parameters:
                playgroundSizeX = playgroundSizeX;
                playgroundSizeY = playgroundSizeY;
                @display("p=202,52");
        }
        flatNetworkConfigurator: FlatNetworkConfigurator {
            @display("p=123,140");
        }
        mgmt: VSimRTIScenarioManager {
            @display("p=57,52;is=s");
        }

        // List of vehicles (dummy).
        // This object is just a dummy to have auto-completion feature available in the omnetpp.ini. 
        // The real vehicle objects will be generated on the fly, when needed.
        veh[0]: Vehicle {
            @dynamic;
            @display("p=309,52");
        }
        
        // List of rsus (dummy).
        // This object is just a dummy to have auto-completion feature available in the omnetpp.ini. 
        // The real rsu objects will be generated on the fly, when needed.
        rsu[0]: Rsu {
            @display("p=309,140");
        }
}
