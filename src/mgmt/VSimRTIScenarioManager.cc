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
#include "VSimRTIUnreliableApp.h"
#include "VSimRTIReliableApp.h"
#include "BgTrafficApp.h"
#include "ChannelCMD.h"
#include "VSimRTIMobility.h"
#include "VSimRTIMobilityCmd_m.h"
#include "VSimRTICommunicationCmd_m.h"
#include "VSimRTIAppPacket_m.h"
#include "VSimRTIScenarioManager.h"

#include <sstream>


Define_Module(VSimRTIScenarioManager);


/**
 * Destructor to clean up Federate.
 */
VSimRTIScenarioManager::~VSimRTIScenarioManager() {

}

/**
 * Initialize members and setup connection to Ambassador.
 */
void VSimRTIScenarioManager::initialize() {

	// Initialize members from ned-files
	debug = par("debug");
	vehModuleType = par("vehModuleType").stdstringValue();
	vehModuleName = par("vehModuleName").stdstringValue();
	rsuModuleType = par("rsuModuleType").stdstringValue();
	rsuModuleName = par("rsuModuleName").stdstringValue();
	btvModuleType = par("btvModuleType").stdstringValue();
	btvModuleName = par("btvModuleName").stdstringValue();
	moduleDisplayString = par("moduleDisplayString").stdstringValue();

	sched = check_and_cast<VSimRTIEventScheduler *>(simulation.getScheduler());
	sched->setMgmtModule(this);

	cc = dynamic_cast<ChannelControl *>(simulation.getModuleByPath("channelControl"));
	if (cc == 0) {
		error("Could not find a ChannelControl module named channelControl");
	}

	EV << "VSimRTIScenarioManager initialized" << endl;
}

/**
 * Process duly received and scheduled commands from VSimRTI on the on hand and
 * received v2x messages from omnet internal application layer on the other hand.
 *
 * @param msg
 * 		(event) message
 */
void VSimRTIScenarioManager::handleMessage(cMessage *msg) {

	int 	nodeId;
	Coord 	position;

	// Mobility dependent commands
	if (strcmp(msg->getName(), "VSimRTIMobilityCmd") == 0) {
		VSimRTIMobilityCmd *cmd = check_and_cast<VSimRTIMobilityCmd *>(msg);
		if (msg->getKind() == CMD::ADD_NODES) {
			for (unsigned int i = 0; i < cmd->getNodeIdArraySize(); i++) {
				nodeId = cmd->getNodeId(i);
				position = cmd->getPosition(i);
				if (debug) EV << "VSimRTIScenarioManager ADD_NODES: " << nodeId << " at position " << position.x << "," << position.y << endl;
				addNode(nodeId, position);
			}
		}
		else if (msg->getKind() == CMD::ADD_RSU_NODES) {
			for (unsigned int i = 0; i < cmd->getNodeIdArraySize(); i++) {
				nodeId = cmd->getNodeId(i);
				position = cmd->getPosition(i);
				if (debug) EV << "VSimRTIScenarioManager ADD_RSU_NODES: " << nodeId << " at position " << position.x << "," << position.y << endl;
				addRsuNode(nodeId, position);
			}
		}
		else if (msg->getKind() == CMD::MOVE_NODES) {
			for (unsigned int i = 0; i < cmd->getNodeIdArraySize(); i++) {
				nodeId = cmd->getNodeId(i);
				position = cmd->getPosition(i);
				if (debug) EV << "VSimRTIScenarioManager MOVE_NODES: " << nodeId << " to position " << position.x << "," << position.y << endl;
				moveNode(nodeId, position);
			}
		}
		else if (msg->getKind() == CMD::REMOVE_NODES) {
			for (unsigned int i = 0; i < cmd->getNodeIdArraySize(); i++) {
				nodeId = cmd->getNodeId(i);
				if (debug) EV << "VSimRTIScenarioManager REMOVE_NODES: " << nodeId << endl;
				removeNode(nodeId);
			}
		}
	}
	// Message dependent commands
	else if (strcmp(msg->getName(), "VSimRTICommunicationCmd") == 0) {
		if (msg->getKind() == CMD::SEND_MESSAGE) {
			if (debug) EV << "VSimRTIScenarioManager SEND_MESSAGE" << endl;
			sendV2xMessage(msg);
		}
	}
	// Simulation finish command
	else if (strcmp(msg->getName(), "VSimRTIFinishCmd") == 0) {
		if (debug) EV << "VSimRTIScenarioManager FINISH" << endl;
		// Delete msg because finish() terminates the program.
		delete msg;
		// Call finish at last event
		finish();
	}
	// Internal messages from vsimrti app layer
	else {
		// Send message back to vsimrti, since it was received from vsimrti udp app
		// (vsimrti tcp app is not activated up to now, as no real use case exists for simulation)
		receiveV2xMessage(msg);
	}
	delete msg;
}

/**
 * Finish simulation.
 */
void VSimRTIScenarioManager::finish() {
	static bool once = false;
	
	if (!once) {
		// toggle the once flag
		once = true;

		// Tidy up and finish simulation
		while (nodes.begin() != nodes.end()) {
			removeNode(nodes.begin()->first);
		}
		sched->endRun();
		EV << "VSimRTIScenarioManager simulation ended" << endl;
		endSimulation();
	}
}

/*
 * Return whether a module is managed (already simulated).
 */
cModule* VSimRTIScenarioManager::getManagedModule(int nodeId) {

	if (nodes.find(nodeId) == nodes.end()) return 0;
	return nodes[nodeId];
}

/**
 * Add module to simulation and management.
 *
 * @param nodeId
 * 		VSimRTI id for simulated node (vehicle)
 * @param type
 * 		type of node (currently vehicle)
 * @param name
 * 		name of node to be handled in cmdenv or tkenv
 * @param displayString
 * 		displayString of node to be displayed in tkenv
 */
void VSimRTIScenarioManager::addModule(int nodeId, std::string type, std::string name, std::string displayString) {

	if (nodes.find(nodeId) != nodes.end()) {
		error("Tried adding duplicate module");
	}

	unsigned int nodeVectorIndex = nodeId;

	cModule* parentmod = getParentModule();
	if (!parentmod) {
		error("Parent module not found");
	}

	cModuleType* nodeType = cModuleType::get(type.c_str());
	if (!nodeType) {
		error("Module type \"%s\" not found", type.c_str());
	}

	cModule* mod = nodeType->create(name.c_str(), parentmod, nodeVectorIndex, nodeVectorIndex);
	mod->finalizeParameters();
	mod->getDisplayString().parse(displayString.c_str());
	mod->buildInside();
	mod->scheduleStart(simTime());
	mod->callInitialize();
	nodes[nodeId] = mod;
}

/**
 * Adds node to simulation, connects to vsimrti controlled udp app
 * and updates initial position.
 *
 * @param nodeId
 * 		VSimRTI id for simulated node
 * @param position
 * 		projected coordinates of first node position
 */
void VSimRTIScenarioManager::addNode(int nodeId, Coord& position) {

	cModule* mod = getManagedModule(nodeId);
	if (mod) {
		error("Tried adding duplicate node (vehicle) %d", nodeId);
	}

	addModule(nodeId, vehModuleType, vehModuleName, moduleDisplayString);
	mod = getManagedModule(nodeId);
	for (cModule::SubmoduleIterator iter(mod); !iter.end(); iter++) {
		cModule* submod = iter();
		VSimRTIUnreliableApp* ua = dynamic_cast<VSimRTIUnreliableApp*>(submod);
		if (!ua) {
			VSimRTIMobility* mm = dynamic_cast<VSimRTIMobility*>(submod);
			if (!mm) continue;
			// Initialize vsimrtimobility module (external id and initial position)
			mm->setExternalId(nodeId);
			mm->setNextPosition(position);
		}
		else {
			// Initialize vsimrtiunreliableapp module (connection)
			this->setGateSize("vSimRTIUdpOut", this->gateSize("vSimRTIUdpOut") + 1);
			this->gate("vSimRTIUdpOut", nodeId)->connectTo(ua->gate("fedIn"));
			this->setGateSize("vSimRTIUdpIn", this->gateSize("vSimRTIUdpIn") + 1);
			ua->gate("fedOut")->connectTo(this->gate("vSimRTIUdpIn", nodeId));
			ua->setExternalId(nodeId);
		}
	}

	EV << "VSimRTIScenarioManager added vehicle " << nodeId << " at position " << position.x << "," << position.y << " at time " << simTime() << endl;
}

/**
 * Adds rsu to simulation, connects to vsimrti controlled udp app
 * and sets position.
 *
 * @param nodeId
 * 		VSimRTI id for simulated rsu
 * @param position
 * 		projected coordinates rsu position
 */
void VSimRTIScenarioManager::addRsuNode(int nodeId, Coord& position) {

	cModule* mod = getManagedModule(nodeId);
	if (mod) {
		error("Tried adding duplicate node (rsu) %d", nodeId);
	}

	addModule(nodeId, rsuModuleType, rsuModuleName, moduleDisplayString);
	mod = getManagedModule(nodeId);
	for (cModule::SubmoduleIterator iter(mod); !iter.end(); iter++) {
		cModule* submod = iter();
		VSimRTIUnreliableApp* ua = dynamic_cast<VSimRTIUnreliableApp*>(submod);
		if (!ua) {
			VSimRTIMobility* mm = dynamic_cast<VSimRTIMobility*>(submod);
			if (!mm) continue;
			// Initialize vsimrtimobility module (external id and initial position)
			mm->setExternalId(nodeId);
			mm->setNextPosition(position);
		}
		else {
			// Initialize vsimrtiunreliableapp module (connection)
			this->setGateSize("vSimRTIUdpOut", this->gateSize("vSimRTIUdpOut") + 1);
			this->gate("vSimRTIUdpOut", nodeId)->connectTo(ua->gate("fedIn"));
			this->setGateSize("vSimRTIUdpIn", this->gateSize("vSimRTIUdpIn") + 1);
			ua->gate("fedOut")->connectTo(this->gate("vSimRTIUdpIn", nodeId));
			ua->setExternalId(nodeId);
		}
	}

	EV << "VSimRTIScenarioManager added rsu " << nodeId << " at position " << position.x << "," << position.y << " at time " << simTime() << endl;
}

/**
 * Updates position of simulated node.
 *
 * @param nodeId
 * 		VSimRTI id for simulated node
 * @param position
 * 		projected coordinates of new node position
 */
void VSimRTIScenarioManager::moveNode(int nodeId, Coord& position) {

	cModule* mod = getManagedModule(nodeId);
	if (!mod) {
//		error("Node %d not mapped", nodeId);
		EV << "WARNING: Node " << nodeId << " not mapped" << endl;
	}
	else {
		for (cModule::SubmoduleIterator iter(mod); !iter.end(); iter++) {
			cModule* submod = iter();
			VSimRTIMobility* mm = dynamic_cast<VSimRTIMobility*>(submod);
			if (!mm) continue;
			mm->setNextPosition(position);
		}

		if (debug) EV << "VSimRTIScenarioManager moved vehicle " << nodeId << " to position " << position.x << "," << position.y << " at time " << simTime() << endl;
	}
}

/**
 * Removes node from simulation.
 *
 * @param nodeId
 * 		VSimRTI id of node to be removed
 */
void VSimRTIScenarioManager::removeNode(int nodeId) {

	cModule* mod = getManagedModule(nodeId);
	if (!mod) {
		error("No node with id %d found", nodeId);
	}
	else {
		cc->unregisterHost(mod);

		nodes.erase(nodeId);
		mod->callFinish();
		mod->deleteModule();

		EV << "VSimRTIScenarioManager removed node " << nodeId << " at time " << simTime() << endl;
	}
}

/**
 * Send udp message from dedicated node.
 *
 * @param nodeId
 * 		Node, which emits the unreliable message
 * @param msgId
 * 		Message, which is sent by node
 */
void VSimRTIScenarioManager::sendV2xMessage(cMessage *msg) {

	VSimRTICommunicationCmd *cmd = check_and_cast<VSimRTICommunicationCmd *>(msg);
	int nodeId = cmd->getNodeId();
	int msgId = cmd->getMsgId();
	AppProtocolType protocol = (AppProtocolType) cmd->getProtocol();

	cModule* mod = getManagedModule(nodeId);
	if (!mod) {
		EV << "WARNING: Node " << nodeId << " not mapped" << endl;
	}
	else {
		char packetName[32];
		sprintf(packetName,"V2xPacket-%d", msgId);
		VSimRTIAppPacket *packet = new VSimRTIAppPacket(packetName, 0);
		packet->setNodeId(nodeId);
		packet->setMsgId(msgId);
		packet->setByteLength(cmd->getLength());
		packet->setDestAddr(cmd->getDestAddr());
		packet->setChannelId(cmd->getChannelId());

		if (cmd->getTtl() != 1)
			EV << "WARNING: up to now only singlehop-broadcast supported" << endl;
		else {
			switch (protocol) {
			case UDP:
				send(packet, "vSimRTIUdpOut", nodeId);
				EV << "VSimRTIScenarioManager send udp message " << msgId << "(" << protocol << ") from node " << nodeId << " at time " << simTime() << endl;
				break;
			case TCP:
				EV << "VSimRTIScenarioManager SEND_MESSAGE: TCP messages not fully supported yet" << endl;
				break;
			default:
				error ("Unsupported application protocol type");
			}
		}
	}
}

/**
 * Receive v2x message in the name of the current node and forward it to vsimrti
 */
void VSimRTIScenarioManager::receiveV2xMessage(cMessage *msg) {

	VSimRTIAppPacket *packet = check_and_cast<VSimRTIAppPacket *>(msg);
	EV << "VSimRTIScenarioManager receive v2x message " << packet->getMsgId() << " on node " << packet->getNodeId() << " at time " << simTime() << endl;
	sched->reportReceivedV2xMessage(msg);
}
