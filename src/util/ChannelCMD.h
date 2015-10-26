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

#ifndef CHANNELCMD_H_
#define CHANNELCMD_H_


/**
 * Collection of implemented commands for communication protocol from OMNeT++ Ambassador to Federate.
 *
 * @author rpr
 */
class CMD {

	public:
		/** New command is expected. */
		static const int NEW;

		/** Initialization of Federate. */
		static const int INIT;

		/** Simulation time advance. */
		static const int ADVANCE_TIME;

		/** Next scheduling event time request. */
		static const int NEXT_EVENT;

		/** Finish complete OMNeT++ simulation. */
		static const int FINISH;

		/** Add new vehicle nodes to simulation. */
		static const int ADD_NODES;

		/** Add new rsu nodes to simulation. */
		static const int ADD_RSU_NODES;

		/** Update positions of currently simulated nodes. */
		static const int MOVE_NODES;

		/** Remove nodes from simulation. */
		static const int REMOVE_NODES;

		/** Transmit info for one dedicated node. */
		static const int NODE_INFO;

		/** Send v2x message for dedicated node. */
		static const int SEND_MESSAGE;

		/** Receive v2x message for dedicated node. */
		static const int RECV_MESSAGE;

		/** End of current command line. */
		static const int END;
};

#endif /* CHANNELCMD_H_ */
