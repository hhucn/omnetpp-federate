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

#ifndef VSIMRTIEXTENDEDMOBILITYNODE_H_
#define VSIMRTIEXTENDEDMOBILITYNODE_H_

#include "ccompoundmodule.h"

class VSimRTIExtendedMobilityNode: public cCompoundModule {
	private:
		char* currentRoadId;
		float currentLanePos;

	public:
		VSimRTIExtendedMobilityNode() : currentRoadId(NULL) {};

		virtual ~VSimRTIExtendedMobilityNode() {
			if (currentRoadId != NULL) delete[] currentRoadId;
		}

		void setRoadId(const char* roadId) {
			if (currentRoadId != NULL) delete[] currentRoadId;

			size_t length = strlen(roadId);
			currentRoadId = new char[length + 1];
			strcpy(currentRoadId, roadId);
		}

		void setLanePos(float lanePos) {
			currentLanePos = lanePos;
		}

		inline const char* const getRoadId() {
			return currentRoadId;
		}

		inline const float getLanePos() {
			return currentLanePos;
		}

		virtual void extendedMobilityUpdated() = 0;
};

#endif /* VSIMRTIEXTENDEDMOBILITYNODE_H_ */
