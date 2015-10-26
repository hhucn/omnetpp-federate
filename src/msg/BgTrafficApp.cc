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

#include "BgTrafficApp.h"


Define_Module(BgTrafficApp);


/**
 * Destructor.
 */
BgTrafficApp::~BgTrafficApp() {

}


/**
 * Initialize method to bind a udp socket to this app layer and schedule first packets.
 */
void BgTrafficApp::initialize(int stage) {

    // because of IPAddressResolver, we need to wait until interfaces are registered,
    // address auto-assignment takes place etc.
    if (stage!=3)
        return;

    EV << "Initializing VSimRTIUnreliableApp stage " << stage << endl;

    localPort = par("localPort");
    destPort = par("destPort");
    packetFreq = ((double) par("packetFreq")) / 1000; // in ms
    packetJitter = ((double) par("packetJitter")) / 1000000; // in us
    packetSize = par("packetSize");

    bindToPort(localPort);

    cMessage *timer = new cMessage("bgSendTimer");
    simtime_t time = simTime() + packetFreq + dblrand()*packetJitter;
    scheduleAt(time, timer);
}

/**
 * Method for sending of background traffic packet.
 */
void BgTrafficApp::handleMessage(cMessage *msg) {

	// Only consider own messages for scheduling of packet sending
	// and ignore received packets from other nodes
	if (msg->isSelfMessage()) {

		// Prepare and send broadcast packet for traffic
		cPacket *packet = new cPacket("BgtPacket", 0);
		packet->setByteLength(packetSize);
		sendToUDP(packet, localPort, IPAddress::ALLONES_ADDRESS, destPort);

		// Schedule next
	    cMessage *timer = new cMessage("bgSendTimer");
		simtime_t time = simTime() + packetFreq + dblrand()*packetJitter;
		scheduleAt(time, timer);
	}
	delete(msg);
}
