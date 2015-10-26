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

class noncobject Coord;

cplusplus {{
    #include "Coord.h"
}}

//
// Messagetype for mobility commands (add_node, move_node, remove_node)
//
message VSimRTIMobilityCmd {
    int 	nodeId[];		// id of vehicle or rsu
    Coord 	position[];		// projected position coordinates for mobility
    string	roadId[];		// id of vehicles current road/edge (directed)
    double	lanePos[];		// position of vehicle on edge
}
