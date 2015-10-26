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

package federate.msg;

import inet.applications.udpapp.UDPApp;

//
// Application layer module for sending of background traffic packets
//
simple BgTrafficApp like UDPApp
{
    parameters:
        int localPort = default(1001);	// local port for udp binding
        int destPort = default(1001);	// destination port for udp messages
    	double packetFreq @unit(ms) = default(100ms);
    	double packetJitter @unit(us)= default(1us);
    	int packetSize = default(500);
    gates:
        input udpIn;
        output udpOut;
}