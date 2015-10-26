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

enum AppProtocolType;

class noncobject Coord;
class noncobject IPAddress;

cplusplus {{
    #include "AppProtocolType.h"
    #include "Coord.h"
    #include "IPAddress.h"
}}

//
// Messagetype for communication command (send_message)
//
message VSimRTICommunicationCmd {
    int			nodeId;							// Node id
    int			protocol enum(AppProtocolType);	// UDP or TCP
    int         channelId;                      // wlan channel
    int			msgId;							// Message id
    
    int			length;							// Length of v2x messages payload
    
    int			ttl;							// TTL for topological sending (or geographical if < 0)
    
    Coord		geoAddrCenter;					// Center coordinate of geographic destination
    float		geoAddrRadius;					// Radius of geographic destination area
    IPAddress	destAddr;						// Destination ip address
}
