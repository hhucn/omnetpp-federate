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

#include "AppProtocolType.h"
#include "ChannelCMD.h"
#include "VSimRTIMobilityCmd_m.h"
#include "VSimRTICommunicationCmd_m.h"
#include "VSimRTIAppPacket_m.h"
#include "VSimRTIEventScheduler.h"

#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>


using namespace std;


/** Reference to scenario management module */
cModule* mgmt;

/** State for simulating phase. */
int simulating = 0;


Register_Class(VSimRTIEventScheduler);
Register_GlobalConfigOption(CFGID_VSIMRTIEVENTSCHEDULER_DEBUG, "vsimrtieventscheduler-debug", CFG_BOOL, false, "Switch for debugprints of scheduler.");
Register_GlobalConfigOption(CFGID_VSIMRTIEVENTSCHEDULER_HOST, "vsimrtieventscheduler-host", CFG_STRING, NULL, "Own hostname for connection with vsimrti.");
Register_GlobalConfigOption(CFGID_VSIMRTIEVENTSCHEDULER_PORT, "vsimrtieventscheduler-port", CFG_INT, 0, "Port for outchannel socket to vsimrti.");


/**
 * SIGINT processing.
 */
void sigproc(int signo) {
	signal (SIGINT, sigproc);
	pthread_exit(NULL);
}

/**
 * Function for for_each call. Deletes the given pointer array.
 * @param roadId Road id string
 */
void deleteRoadId(char* roadId) { delete[] roadId; }

/**
 * Constructor.
 */
VSimRTIEventScheduler::VSimRTIEventScheduler() : cScheduler() {

}

/**
 * Destructor.
 */
VSimRTIEventScheduler::~VSimRTIEventScheduler() {

}

/**
 * Init method, called at simulation start.
 */
void VSimRTIEventScheduler::startRun() {

	cout << "VSimRTIEventScheduler started" << endl;

	// Initialize members
	debug = ev.getConfig()->getAsBool(CFGID_VSIMRTIEVENTSCHEDULER_DEBUG);
	host = ev.getConfig()->getAsString(CFGID_VSIMRTIEVENTSCHEDULER_HOST);
	port = ev.getConfig()->getAsInt(CFGID_VSIMRTIEVENTSCHEDULER_PORT);

	pthread_mutex_init(&waitMutex, NULL);
	pthread_cond_init(&waitCond, NULL);

	// Create thread, which receives time advance grants from vsimrti
	if (pthread_create(&receiveInteractionsThread, NULL, &receiveInteractions, this) != 0) {
		cRuntimeError("VSimRTIEventScheduler FAILURE (creation of waitTimeAdvanceThread not possible)");
	}
}

/**
 * Final method, called at simulation end.
 */
void VSimRTIEventScheduler::endRun() {
	static bool once = false;

	if (!once) {
		// toggle the once flag
		once = true;

		disconnectFromAmbassador();
		pthread_cancel(receiveInteractionsThread);
		pthread_join(receiveInteractionsThread, NULL);
		pthread_mutex_destroy(&waitMutex);
		//pthread_cond_destroy(&waitCond);
		//pthread_exit(NULL);
		cout << "VSimRTIEventScheduler ended" << endl;
	}
}

/**
 * Scheduler method, which delivers next event, if time advance is already granted by vsimrti,
 * otherwise it will wait for vsimrti controlling simulation time to be advanced.
 *
 * @return
 * 		nextEvent
 */
cMessage* VSimRTIEventScheduler::getNextEvent() {

	cMessage *msg;
	// no messages in queue
	while (!(msg = sim->msgQueue.peekFirst())) {
		// simulation to be started
		if (simulating == 0) {
			pthread_mutex_lock(&waitMutex);
			pthread_mutex_unlock(&waitMutex);
		}
		// simulating
		else if (simulating == 1) {
			pthread_mutex_lock(&waitMutex);
			reportNextEvent(currSimTime);
			pthread_cond_wait(&waitCond, &waitMutex);
			pthread_mutex_unlock(&waitMutex);
		}
		// simulation finished
		else if (simulating == 2) {
			return NULL;
		}
	}
	// message in queue with time stamp later than logical simulation time
	while ((msg = sim->msgQueue.peekFirst())->getArrivalTime() > currSimTime) {
		pthread_mutex_lock(&waitMutex);
		reportNextEvent(msg->getArrivalTime());
		pthread_cond_wait(&waitCond, &waitMutex);
		pthread_mutex_unlock(&waitMutex);
	}
	msg = sim->msgQueue.peekFirst();
	if (debug) EV << "VSimRTIEventScheduler scheduled EVENT: t=" << msg->getArrivalTime().str() << ", currSimTime=" << currSimTime.str() << endl;
	return msg;
}

/**
 * Sets reference to scenario management module.
 */
void VSimRTIEventScheduler::setMgmtModule(cModule *mod) {

	mgmt = mod;
}

/**
 * Connect to Ambassador.
 */
void VSimRTIEventScheduler::connectToAmbassador() {

	int actPort;

	// Use ClientServerChannel as abstraction for communication with Ambassasdor
	outChannel = new ClientServerChannel();
	actPort = outChannel->prepareConnection(host, port);
	if (actPort != port)
		if (debug) EV << "VSimRTIEventScheduler bound different port " << actPort << " instead of port " << port << endl;
	pthread_mutex_lock(&waitMutex);
	cout << "VSimRTIEventScheduler connecting on OutPort=" << actPort << endl;
	pthread_mutex_unlock(&waitMutex);
	if (debug) EV << "VSimRTIEventScheduler connecting on OutPort=" << actPort << endl;
	outChannel->connect();

	cmdChannel = new ClientServerChannel();
	actPort = cmdChannel->prepareConnection(host, 0);
	if (debug) EV << "VSimRTIEventScheduler connecting on CmdPort=" << actPort << endl;
	outChannel->writeCommand(CMD::INIT);
	outChannel->writeNodeId(actPort);
	cmdChannel->connect();

	if (debug) EV << "VSimRTIEventScheduler connected to Ambassador" << endl;
}

/**
 * Closes socket connections.
 */
void VSimRTIEventScheduler::disconnectFromAmbassador() {

	delete(cmdChannel);
	delete(outChannel);
}

/**
 * Reporting method, for next event to be allowed to schedule.
 *
 * @param nextSimTime
 * 		next event to be reported to vsimrti to advance time until then
 */
void VSimRTIEventScheduler::reportNextEvent(simtime_t nextSimTime) {

	// Report next event
	if (debug) EV << "VSimRTIEventScheduler request NEXT_EVENT: t=" << nextSimTime.str() << endl;
	outChannel->writeCommand(CMD::NEXT_EVENT);
	outChannel->writeTime(nextSimTime);
}

/**
 * Reporting method, for received v2x message.
 * is a public interface for access to clientserverchannel and is called by vsimrtiscenariomanager
 *
 * @param msg
 * 		received v2x message on dedicated node to be reported to vsimrti
 */
void VSimRTIEventScheduler::reportReceivedV2xMessage(cMessage *msg) {

	VSimRTIAppPacket *packet = check_and_cast<VSimRTIAppPacket *>(msg);
	if (debug) EV << "VSimRTIEventScheduler report RECV_MESSAGE: t=" << packet->getArrivalTime().str() << ", RecNodeId=" << packet->getNodeId() << ", MsgId=" << packet->getMsgId() << endl;
	outChannel->writeCommand(CMD::RECV_MESSAGE);
	outChannel->writeTime(packet->getArrivalTime());
	outChannel->writeNodeId(packet->getNodeId());
	outChannel->writeMessageId(packet->getMsgId());
	// write the receive signal strength (currently not supported)
	// TODO: write the receive signal strength
	outChannel->writeMessageReceiveSignalStrength(0);
}

/**
 * Do protocol logic (wait for new and process commands from Ambassador).
 */
void* VSimRTIEventScheduler::receiveInteractions(void* p) {

	signal(SIGINT, sigproc);

	VSimRTIEventScheduler* th = (VSimRTIEventScheduler*) p;

	int 		command;
	simtime_t 	time;

	int 			nodeId;
	vector<int> 	nodeIdVec;
	Coord 			position;
	vector<Coord> 	positionVec;
	char*			roadId;
	vector<char*>	roadIdVec;
	double			lanePos;
	vector<double>	lanePosVec;

	// Setup connection to OMNeT++ ambassador (as a server)
	th->connectToAmbassador();

	// Get initial command with simulation times from ambassador
	if (th->debug) EV << "VSimRTIEventScheduler wait INIT" << endl;
	command = th->cmdChannel->readCommand();
	if (command == CMD::INIT) {
		pthread_mutex_lock(&(th->waitMutex));

		// Initialize simulation times
		th->startTime = th->cmdChannel->readTime();
		th->stopTime = th->cmdChannel->readTime();
		if (th->debug) EV << "VSimRTIEventScheduler simulation times: Start (" << th->startTime.str() << "s), Stop (" << th->stopTime.str() << "s)" << endl;

		th->currSimTime = th->startTime;
		simulating = 1;
		pthread_cond_signal(&(th->waitCond));

		if (th->debug) EV << "VSimRTIEventScheduler successfully initialized" << endl;
		th->cmdChannel->writeStatus(true, "");
		pthread_mutex_unlock(&(th->waitMutex));
	}
	else {
		th->cmdChannel->writeStatus(false, "");
		cRuntimeError("VSimRTIEventScheduler FAILURE (unexpected command %d)", command);
	}

	// wait until vsimrti
	while(1) {
		// Receive new command from Ambassador
		if (th->debug) EV << "VSimRTIEventScheduler wait new command" << endl; // TODO Makes simulation very instable (under windows), when activated. Why???
		command = th->cmdChannel->readCommand();
		if (th->debug) EV << "VSimRTIEventScheduler received command: " << command << endl; // TODO Makes simulation very instable (under windows), when activated. Why???

		// End simulation immediately, when command is finish command
		if (command == CMD::FINISH) {
			pthread_mutex_lock(&(th->waitMutex));
			if (th->debug) EV << "VSimRTIEventScheduler FINISH" << endl;

			cMessage *finMessage = new cMessage("VSimRTIFinishCmd", 22);
			finMessage->setTimestamp(th->currSimTime);
			finMessage->setArrivalTime(th->currSimTime);
			finMessage->setArrival(mgmt, -1);
			simulation.msgQueue.insert(finMessage);
			simulation.msgQueue.sort();
			simulating = 2;
//			th->cmdChannel->writeStatus(false, "");
			pthread_cond_signal(&(th->waitCond));
			pthread_mutex_unlock(&(th->waitMutex));
//			pthread_exit(NULL);
			break;
		}
		else {
			// Otherwise get time for processing of received command
			time = th->cmdChannel->readTime();
			if (th->debug) EV << "VSimRTIEventScheduler received time: " << time.str() << endl;
		}

		// Process duly received command
		// Mobility dependent commands
		if (command == CMD::ADD_NODES) {
			pthread_mutex_lock(&(th->waitMutex));
			while (th->cmdChannel->readCommand() != CMD::END) {
				nodeId = th->cmdChannel->readId();
				position = th->cmdChannel->readCoordinates();
				roadId = th->cmdChannel->readRoadId();
				lanePos = th->cmdChannel->readLanePosition();
				if (th->debug) EV << "VSimRTIEventScheduler ADD_NODES: " << nodeId << " at position " << position.x << "," << position.y << " on lane " << roadId << " with distance " << lanePos << endl;
				nodeIdVec.push_back(nodeId);
				positionVec.push_back(position);
				roadIdVec.push_back(roadId);
				lanePosVec.push_back(lanePos);
			}
			unsigned int numNodes = nodeIdVec.size();
			if (numNodes > 0) {
				VSimRTIMobilityCmd *cmdMessage = new VSimRTIMobilityCmd("VSimRTIMobilityCmd", CMD::ADD_NODES);

				cmdMessage->setTimestamp(time);
				cmdMessage->setArrivalTime(time);
				cmdMessage->setArrival(mgmt, -1);
				cmdMessage->setNodeIdArraySize(numNodes);
				cmdMessage->setPositionArraySize(numNodes);
				cmdMessage->setRoadIdArraySize(numNodes);
				cmdMessage->setLanePosArraySize(numNodes);
				for (unsigned int i = 0; i < numNodes; i++) {
					cmdMessage->setNodeId(i, nodeIdVec[i]);
					cmdMessage->setPosition(i, positionVec[i]);
					cmdMessage->setRoadId(i, roadIdVec[i]);
					cmdMessage->setLanePos(i, lanePosVec[i]);
				}

				simulation.msgQueue.insert(cmdMessage);
				simulation.msgQueue.sort();

				// Delete roadId string memory
				std::for_each(roadIdVec.begin(), roadIdVec.end(), deleteRoadId);

				nodeIdVec.clear();
				positionVec.clear();
				roadIdVec.clear();
				lanePosVec.clear();
			}
			if (th->debug) EV << "VSimRTIEventScheduler finished processing of command" << endl;
			th->cmdChannel->writeStatus(true, "");
			pthread_mutex_unlock(&(th->waitMutex));
		}
		else if (command == CMD::ADD_RSU_NODES) {
			pthread_mutex_lock(&(th->waitMutex));
			while (th->cmdChannel->readCommand() != CMD::END) {
				nodeId = th->cmdChannel->readId();
				position = th->cmdChannel->readCoordinates();
				if (th->debug) EV << "VSimRTIEventScheduler ADD_RSU_NODES: " << nodeId << " at position " << position.x << "," << position.y << endl;
				nodeIdVec.push_back(nodeId);
				positionVec.push_back(position);
			}
			unsigned int numNodes = nodeIdVec.size();
			if (numNodes > 0) {
				VSimRTIMobilityCmd *cmdMessage = new VSimRTIMobilityCmd("VSimRTIMobilityCmd", CMD::ADD_RSU_NODES);

				cmdMessage->setTimestamp(time);
				cmdMessage->setArrivalTime(time);
				cmdMessage->setArrival(mgmt, -1);
				cmdMessage->setNodeIdArraySize(numNodes);
				cmdMessage->setPositionArraySize(numNodes);
				for (unsigned int i = 0; i < numNodes; i++) {
					cmdMessage->setNodeId(i, nodeIdVec[i]);
					cmdMessage->setPosition(i, positionVec[i]);
				}

				simulation.msgQueue.insert(cmdMessage);
				simulation.msgQueue.sort();

				nodeIdVec.clear();
				positionVec.clear();
			}
			if (th->debug) EV << "VSimRTIEventScheduler finished processing of command" << endl;
			th->cmdChannel->writeStatus(true, "");
			pthread_mutex_unlock(&(th->waitMutex));
		}
		else if (command == CMD::MOVE_NODES) {
			pthread_mutex_lock(&(th->waitMutex));
			while (th->cmdChannel->readCommand() != CMD::END) {
				nodeId = th->cmdChannel->readId();
				position = th->cmdChannel->readCoordinates();
				roadId = th->cmdChannel->readRoadId();
				lanePos = th->cmdChannel->readLanePosition();
				if (th->debug) EV << "VSimRTIEventScheduler MOVE_NODES: " << nodeId << " to position " << position.x << "," << position.y << " on lane " << roadId << " with distance " << lanePos << endl;
				nodeIdVec.push_back(nodeId);
				positionVec.push_back(position);
				roadIdVec.push_back(roadId);
				lanePosVec.push_back(lanePos);
			}
			unsigned int numNodes = nodeIdVec.size();
			if (numNodes > 0) {
				VSimRTIMobilityCmd *cmdMessage = new VSimRTIMobilityCmd("VSimRTIMobilityCmd", CMD::MOVE_NODES);

				cmdMessage->setTimestamp(time);
				cmdMessage->setArrivalTime(time);
				cmdMessage->setArrival(mgmt, -1);
				cmdMessage->setNodeIdArraySize(numNodes);
				cmdMessage->setPositionArraySize(numNodes);
				cmdMessage->setRoadIdArraySize(numNodes);
				cmdMessage->setLanePosArraySize(numNodes);
				for (unsigned int i = 0; i < numNodes; i++) {
					cmdMessage->setNodeId(i, nodeIdVec[i]);
					cmdMessage->setPosition(i, positionVec[i]);
					cmdMessage->setRoadId(i, roadIdVec[i]);
					cmdMessage->setLanePos(i, lanePosVec[i]);
				}

				simulation.msgQueue.insert(cmdMessage);
				simulation.msgQueue.sort();

				// Delete roadId string memory
				std::for_each(roadIdVec.begin(), roadIdVec.end(), deleteRoadId);

				nodeIdVec.clear();
				positionVec.clear();
				roadIdVec.clear();
				lanePosVec.clear();
			}
			if (th->debug) EV << "VSimRTIEventScheduler finished processing of command" << endl;
			th->cmdChannel->writeStatus(true, "");

			pthread_mutex_unlock(&(th->waitMutex));
		}
		else if (command == CMD::REMOVE_NODES) {
			pthread_mutex_lock(&(th->waitMutex));
			while (th->cmdChannel->readCommand() != CMD::END) {
				nodeId = th->cmdChannel->readId();
				EV << "VSimRTIEventScheduler REMOVE_NODES: " << nodeId << endl;
				nodeIdVec.push_back(nodeId);
			}
			unsigned int numNodes = nodeIdVec.size();
			if (numNodes > 0) {
				VSimRTIMobilityCmd *cmdMessage = new VSimRTIMobilityCmd("VSimRTIMobilityCmd", CMD::REMOVE_NODES);

				cmdMessage->setTimestamp(time);
				cmdMessage->setArrivalTime(time);
				cmdMessage->setArrival(mgmt, -1);
				cmdMessage->setNodeIdArraySize(numNodes);
				for (unsigned int i = 0; i < numNodes; i++) {
					cmdMessage->setNodeId(i, nodeIdVec[i]);
				}

				simulation.msgQueue.insert(cmdMessage);
				simulation.msgQueue.sort();

				nodeIdVec.clear();
			}
			if (th->debug) EV << "VSimRTIEventScheduler finished processing of command" << endl;
			th->cmdChannel->writeStatus(true, "");

			pthread_mutex_unlock(&(th->waitMutex));
		}

		// Message dependent commands
		else if (command == CMD::SEND_MESSAGE) {
			pthread_mutex_lock(&(th->waitMutex));

			VSimRTICommunicationCmd *comMessage = new VSimRTICommunicationCmd("VSimRTICommunicationCmd", CMD::SEND_MESSAGE);
			comMessage->setTimestamp(time);
			comMessage->setArrivalTime(time);
			comMessage->setArrival(mgmt, -1);
			comMessage->setNodeId(th->cmdChannel->readId());
			comMessage->setProtocol(th->cmdChannel->readProtocol());
			comMessage->setChannelId(th->cmdChannel->readChannelId());
			comMessage->setMsgId(th->cmdChannel->readMessageId());
			comMessage->setLength(th->cmdChannel->readV2xMessageLength());

			//read the marker
			int marker = th->cmdChannel->readAddressMarker();
			if (marker == 0) {
				comMessage->setDestAddr(th->cmdChannel->readIpAddress());
				//set pseudo Ttl
				comMessage->setTtl(-1);
				//not used yet (center position)
				Coord center = th->cmdChannel->readCoordinates();
				//read the radius
				comMessage->setGeoAddrRadius(th->cmdChannel->readRoutingRadius());
				//projectedPosition
				comMessage->setGeoAddrCenter(th->cmdChannel->readCoordinates());
			} else if (marker == 1) {
				comMessage->setDestAddr(th->cmdChannel->readIpAddress());
				int ttl = th->cmdChannel->readRoutingTtl();
				comMessage->setTtl(ttl);
			} else {
				cRuntimeError("VSimRTIEventScheduler FAILURE (Illegal (unknown) address extension. %d)", marker);
			}

			if (th->debug) EV << "VSimRTIEventScheduler SEND_MESSAGE: from=" << comMessage->getNodeId() << ", id=" << comMessage->getMsgId() << ", prot=" << comMessage->getProtocol() << endl;

			simulation.msgQueue.insert(comMessage);
			simulation.msgQueue.sort();

			if (th->debug) EV << "VSimRTIEventScheduler finished processing of command" << endl;
			th->cmdChannel->writeStatus(true, "");

			pthread_mutex_unlock(&(th->waitMutex));
		}

		// Time advance grant
		else if (command == CMD::ADVANCE_TIME) {
			pthread_mutex_lock(&(th->waitMutex));
			th->currSimTime = time;
			if (th->debug) EV << "VSimRTIEventScheduler ADVANCE_TIME: " << th->currSimTime.str() << endl;
			pthread_cond_signal(&(th->waitCond));
			pthread_mutex_unlock(&(th->waitMutex));
		}

		else {
			th->cmdChannel->writeStatus(false, "");
			cRuntimeError("VSimRTIEventScheduler FAILURE (received unknown command %d)", command);
		}
	} // end while
	return NULL;
}
