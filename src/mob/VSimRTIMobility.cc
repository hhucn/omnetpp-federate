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

#include "VSimRTIMobility.h"


Define_Module(VSimRTIMobility);


/**
 * Initializes also parent module BasicMobility.
 *
 * @param stage
 */
void VSimRTIMobility::initialize(int stage) {

	EV << "Initialize VSimRTIMobility stage " << stage << endl;
	BasicMobility::initialize(stage);

	if (stage == 1) {
		debug = par("debug");
	}
}

/**
 * Unused but mandatory stub, since all interactions are controlled by VSimRTI
 * respectively OMNeT++ Federate.
 */
void VSimRTIMobility::handleSelfMsg(cMessage *msg) {

}

/**
 * Mandatory finish, but clean up is done in OMNeT++ Federate.
 */
void VSimRTIMobility::finish() {

}

/**
 * Sets VSimRTI controlled nodeId for this module.
 *
 * @param externalId
 * 		nodeId as managed within VSimRTI
 */
void VSimRTIMobility::setExternalId(int externalId) {
	this->externalId = externalId;
}

/**
 * Provides mobility update with projected coordinates from VSimRTI
 *
 * @param nextPos
 * 		next position to be updated as (x,y)
 */
void VSimRTIMobility::setNextPosition(Coord& nextPos) {

	// Update position coordinates
	pos.x = nextPos.x;
	pos.y = nextPos.y;

	// Handle case, when node leaves simulated playground, which should
	// definitely not happen, since VSimRTI cares about correct positions
	Coord dummyCoord;
	double dummyDouble;
	handleIfOutside(RAISEERROR, dummyCoord, dummyCoord, dummyDouble);
	// Trigger position update in BasicMobility
	updatePosition();
}
