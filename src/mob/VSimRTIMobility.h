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

#ifndef VSIMRTIMOBILITY_H_
#define VSIMRTIMOBILITY_H_


#include <omnetpp.h>

#include "BasicMobility.h"
#include "ModuleAccess.h"


/**
 * Mobility module for nodes, which are controlled by mobility input from VSimRTI.
 *
 * @author rpr
 */
class INET_API VSimRTIMobility : public BasicMobility {

	public:
		/** Constructor. */
		VSimRTIMobility() : BasicMobility() {}

		/** Initialize method to also initialize BasicMobility. */
		virtual void initialize(int stage);

		/** Mandatory stub of handleSelfMsg method. */
		virtual void handleSelfMsg(cMessage *msg);

		/** Mandatory finish method. */
		virtual void finish();

		/** setNextPosition method to update node positions. */
		virtual void setNextPosition(Coord& nextPos);

		/** setExternalId method from VSimRTI id to OMNeT++ internal id. */
		virtual void setExternalId(int externalId);

	private:
		/** Logging level of debug prints. */
		bool debug;

		/** VSimRTI controlled node id. */
		int externalId;
};


#endif /* VSIMRTIMOBILITY_H_ */
