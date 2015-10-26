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
// Application layer module for sending and receiving of udp messages
// controlled by VSimRTI applications.
//
simple VSimRTIUnreliableApp like UDPApp
{
    parameters:
        int localPort = default(1001);	// local port for udp binding
        int destPort = default(1001);	// destination port for udp messages
        double maxProcDelay = default(1e-3);	// max val for random delay (synchronous dcf)
    gates:
        input fedIn;
        input udpIn;
        output fedOut;
        output udpOut;
}