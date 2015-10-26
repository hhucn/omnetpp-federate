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

#ifndef __BGTRAFFICAPP_H__
#define __BGTRAFFICAPP_H__

#include <omnetpp.h>

#include "UDPAppBase.h"


/**
 * Application Layer for Background Traffic with UDP messages.
 */
class INET_API BgTrafficApp : public UDPAppBase {

	public:
		/** Destructor. */
		virtual ~BgTrafficApp();

		/** Initialize method to bind a udp socket to this app layer and schedule first packets. */
		virtual void initialize(int stage);

		/** Event scheduling method, especially for sending background packets. */
		virtual void handleMessage(cMessage *msg);

	protected:
		/** Returns stage no. 4, because ipaddressresolver needs 3 init steps */
		virtual int numInitStages() const {return 4;}

	private:
		/** Local port for udp binding. */
		int localPort;

		/** Destination port for sending of udp packets. */
		int destPort;

		/** Sending frequency for background traffic packets. */
		double packetFreq;

		/** Jitter of packets for background traffic
		 *  for more realistic behavior and to avoid simultaneous MAC messages. */
		double packetJitter;

		/** Size of background traffic packets. */
		int packetSize;
};

#endif
