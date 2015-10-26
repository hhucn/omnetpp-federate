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

#ifndef VSIMRTIUNRELIABLEAPP_H_
#define VSIMRTIUNRELIABLEAPP_H_


#include <omnetpp.h>

#include "UDPAppBase.h"


/**
 * Application Layer for UDP messages for VSimRTI simulated nodes.
 */
class INET_API VSimRTIUnreliableApp : public UDPAppBase {

	public:
		/** Destructor. */
		virtual ~VSimRTIUnreliableApp();

		/** Initialize method to bind a udp socket to this app layer. */
		virtual void initialize(int stage);

		/** setExternalId method from VSimRTI id to OMNeT++ internal id. */
		virtual void setExternalId(int externalId);

		/** Event scheduling method, especially for receiving of udp packets. */
		virtual void handleMessage(cMessage *msg);

	protected:
		/** Returns stage no. 4, because ipaddressresolver needs 3 init steps */
		virtual int numInitStages() const {return 4;}

		/** Simulate processing delay on application layer to avoid problem of dcf in mac layer
		 *  with completely synchronous message sending. */
		virtual void sendDelayedToUDP(cPacket *msg, int srcPort, const IPvXAddress& destAddr, int destPort, double delay);


	private:
		/** VSimRTI controlled node id. */
		int externalId;

		/** Local port for udp binding. */
		int localPort;

		/** Destination port for sending of udp packets. */
		int destPort;

		/** Maximum value for random processing delay which is introduced to avoid
		 *  completely synchronous message sending on mac layer. */
		double maxProcDelay;

		/** Method for sending of unreliable udp packets,
		 *  triggered from VSimRTIScenarioManager and hence from VSimRTI. */
		virtual void sendPacket(cMessage *msg);

		/** Receive of udp packets and forwarding to VSimRTI applications. */
		virtual void receivePacket(cMessage *msg);
};

#endif /* VSIMRTIUNRELIABLEAPP_H_ */
