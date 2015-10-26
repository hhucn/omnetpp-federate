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

package federate.mgmt;

//
// Federate module for simulation coupling of OMNeT++ to VSimRTI.
//
simple VSimRTIScenarioManager
{
	parameters:
        bool debug = default(false);  			// debug level for logging
        
        string vehModuleType = default("federate.simulations.Vehicle"); // module type to be used in the simulation for each managed vehicle
        string vehModuleName = default("veh");  	// module name to be used in the simulation for each managed vehicle
        string rsuModuleType = default("federate.simulations.Rsu"); // module type to be used in the simulation for each managed vehicle
        string rsuModuleName = default("rsu");  	// module name to be used in the simulation for each managed vehicle
        string btvModuleType = default("federate.simulations.BtVehicle"); // module type to be used in the simulation for each managed vehicle
        string btvModuleName = default("btv");  	// module name to be used in the simulation for each managed vehicle
        string moduleDisplayString = default("i=misc/node2;is=vs;r=0,,#707070,1");  // module displayString to be used in the simulation for each managed
    gates:
        input vSimRTIUdpIn[];
        output vSimRTIUdpOut[];
}
