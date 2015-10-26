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

#include "VSimRTIAppPacket_m.h"
#include "UDPControlInfo_m.h"
#include "VSimRTIUnreliableApp.h"


Define_Module(VSimRTIUnreliableApp);


/**
 * Destructor.
 */
VSimRTIUnreliableApp::~VSimRTIUnreliableApp() {

}

/**
 * Initialize method to bind a udp socket to this app layer.
 */
void VSimRTIUnreliableApp::initialize(int stage) {

    // because of IPAddressResolver, we need to wait until interfaces are registered,
    // address auto-assignment takes place etc.
    if (stage!=3)
        return;

    EV << "Initializing VSimRTIUnreliableApp stage " << stage << endl;

    localPort = par("localPort");
    destPort = par("destPort");
    maxProcDelay = par("maxProcDelay");

    bindToPort(localPort);
}

/**
 * Sets VSimRTI controlled nodeId for this module.
 *
 * @param externalId
 * 		nodeId as managed within VSimRTI
 */
void VSimRTIUnreliableApp::setExternalId(int externalId) {
	this->externalId = externalId;
}

/**
 * Event scheduling method, especially for receiving of udp packets.
 */
void VSimRTIUnreliableApp::handleMessage(cMessage *msg) {

    // from federate
	if(msg->arrivedOn("fedIn")) {
		sendPacket(msg);
	}

	// from radio
	else if(msg->arrivedOn("udpIn")) {
		// Ignore packets from background traffic
		if (strcmp(msg->getName(), "BgtPacket") != 0) {
			receivePacket(msg);
		}
	}

	delete msg;
}

/**
 * Simulate processing delay on application layer to avoid problem of dcf in mac layer
 * with completely synchronous message sending.
 */
void VSimRTIUnreliableApp::sendDelayedToUDP(cPacket *msg, int srcPort, const IPvXAddress& destAddr, int destPort, double delay)
{
    // send message to UDP, with the appropriate control info attached
    msg->setKind(UDP_C_DATA);

    VSimRTIAppPacket *packet = check_and_cast<VSimRTIAppPacket *>(msg);
    int channelId = packet->getChannelId();

    UDPControlInfo *ctrl = new UDPControlInfo();
    ctrl->setSrcPort(srcPort);
    ctrl->setDestAddr(destAddr);
    ctrl->setDestPort(destPort);
    ctrl->setChannelId(channelId);
    msg->setControlInfo(ctrl);

    EV << "Sending packet: ";
    printPacket(msg);

    sendDelayed(msg, delay, "udpOut");
}

/**
 * Method for sending of unreliable udp packets,
 * triggered from VSimRTIScenarioManager and hence from VSimRTI.
 */
void VSimRTIUnreliableApp::sendPacket(cMessage *msg) {

	EV << "VSimRTIUDP send packet" << endl;

	VSimRTIAppPacket *packet = check_and_cast<VSimRTIAppPacket *>(msg);
	IPvXAddress destAddr = packet->getDestAddr();
	int channelId = packet->getChannelId();

	double delay = dblrand() * maxProcDelay;
	sendDelayedToUDP(PK(msg->dup()), localPort, destAddr, destPort, delay);
}

/**
 * Receive of udp packets and forwarding to VSimRTI applications.
 */
void VSimRTIUnreliableApp::receivePacket(cMessage *msg) {

	EV << "VSimRTIUDP received packet: ";
	printPacket(PK(msg));

	VSimRTIAppPacket *packet = check_and_cast<VSimRTIAppPacket *>(msg);
	EV << "VSimRTIUDP: srcNodeId " << packet->getNodeId() << ", msgId " << packet->getMsgId() << endl;
	packet->setNodeId(this->externalId);
	send(packet->dup(), "fedOut");
}
