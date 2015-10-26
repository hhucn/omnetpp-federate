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

import inet.applications.tcpapp.TCPApp;

//
// Application layer module tcp applications controlled by VSimRTI.
//
simple VSimRTIReliableApp like TCPApp
{
    parameters:
      	string address = default(""); 			// may be left empty ("")
        int port = default(-1); 				// port number to listen on
        string connectAddress = default("");  	// server address (may be symbolic)
        int connectPort = default(1000); 		// port number to connect to
    gates:
        input fedIn;
        input tcpIn;
        output fedOut;
        output tcpOut;
}