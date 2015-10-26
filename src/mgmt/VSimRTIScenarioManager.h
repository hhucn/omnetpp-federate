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

#ifndef VSIMRTISCENARIOMANAGER_H_
#define VSIMRTISCENARIOMANAGER_H_


#include <omnetpp.h>

#include "INETDefs.h"
#include "ChannelControl.h"
#include "ModuleAccess.h"

#include "VSimRTIEventScheduler.h"
#include "ClientServerChannel.h"


using namespace std;


/**
 * Federate module for simulation coupling of OMNeT++ to VSimRTI.
 *
 * @author rpr
 */
class VSimRTIScenarioManager : public cSimpleModule {

	public:
		/** Destructor. */
		virtual ~VSimRTIScenarioManager();

		/** Initialization of members and setup of socket-connection to OMNeT++ Ambassador. */
		virtual void initialize();

		/** Finish of simulation with clean up. */
		virtual void finish();

		/** Scheduling of events for byte protocol between Ambassador and Federate. */
		virtual void handleMessage(cMessage *msg);

	private:
		/** Logging level for debug prints. */
		bool debug;

		/** Module type to be used in the simulation for each managed vehicle. */
		std::string vehModuleType;

		/** Module name to be used in the simulation for each managed vehicle. */
		std::string vehModuleName;

		/** Module type to be used in the simulation for each managed vehicle. */
		std::string rsuModuleType;

		/** Module name to be used in the simulation for each managed vehicle. */
		std::string rsuModuleName;

		/** Module type to be used in the simulation for each managed vehicle. */
		std::string btvModuleType;

		/** Module name to be used in the simulation for each managed vehicle. */
		std::string btvModuleName;

		/** Module displayString to be used in the simulation for each managed node. */
		std::string moduleDisplayString;

		/** Hashmap for storage of simulated vehicles and rsus. */
		std::map<int, cModule*> nodes;

		/** Reference to scheduler. */
		VSimRTIEventScheduler* sched;

		/** Reference to channelControl module. */
		ChannelControl* cc;

		/** Query for managed modules. */
		virtual cModule* getManagedModule(int nodeId);

		/** Introduction of new module. */
		virtual void addModule(int nodeId, std::string type, std::string name, std::string displayString);

		/** Adding method for new node, which also updates first position of node. */
		virtual void addNode(int nodeId, Coord& position);

		/** Adding method for new rsu, which also sets position of rsu. */
		virtual void addRsuNode(int nodeId, Coord& position);

		/** Update method for node positions. */
		virtual void moveNode(int nodeId, Coord& position);

		/** Remove and clean up method for finished nodes. */
		virtual void removeNode(int nodeId);

		/** Send udp message from dedicated node. */
		virtual void sendV2xMessage(cMessage *msg);

		/** Receive v2x message in the name of the current node and forward it to vsimrti */
		virtual void receiveV2xMessage(cMessage *msg);
};

#endif /* OMNETPPFEDERATE_H_ */
