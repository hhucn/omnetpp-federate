//
// (c) 2014 Raphael Bialon <Raphael.Bialon@hhu.de>
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

package federate.simulations.tbus;

import tbus.ned.TbusChannelControl;
import federate.mgmt.VSimRTIScenarioManager;
import inet.networklayer.autorouting.FlatNetworkConfigurator;

network Simulation
{
    parameters:
        double playgroundSizeX;
        double playgroundSizeY;

        @display("bgb=1920,464;bgi=maps/sumo-background,s;bgs=1");
    submodules:
        channelControl: TbusChannelControl {            
            parameters:
                playgroundSizeX = playgroundSizeX;
                playgroundSizeY = playgroundSizeY;
                @display("p=1310,187");
        }
        mgmt: VSimRTIScenarioManager {
            @display("p=1310,119");
        }
        configurator: FlatNetworkConfigurator {
            @display("p=1310,46");
        }
}
