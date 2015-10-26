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

#ifndef VSIMRTIEVENTSCHEDULER_H_
#define VSIMRTIEVENTSCHEDULER_H_


#include <pthread.h>
#include <signal.h>

#include <omnetpp.h>

#include "ClientServerChannel.h"


/**
 * Scheduler module for timeadvance nextevent mechanism
 * used in synchronization of vsimrti and omnet++.
 *
 * @author rpr
 */
class VSimRTIEventScheduler : public cScheduler {

	public:
		/** Constructor. */
		VSimRTIEventScheduler();

		/** Destructor. */
		virtual ~VSimRTIEventScheduler();

		/** Init method, called at simulation start. */
		virtual void startRun();

		/** Final method, called at simulation end. */
		virtual void endRun();

		/** Scheduler method itself. */
		virtual cMessage* getNextEvent();

		/** Sets reference to scenario management module. */
		virtual void setMgmtModule(cModule *mod);

		/** Reports the reception of v2x message back to vsimrti */
		virtual void reportReceivedV2xMessage(cMessage *msg);

	private:
		/** Logging level for debug prints. */
		bool debug;

		/** Own hostname (hostaddress) for setup of server socket. */
		std::string host;

		/** Initial port (for outchannel) to listen on for new connections. */
		int port;

		/** Reference to communication cmdChannel for reading. */
		ClientServerChannel* cmdChannel;

		/** Reference to communication cmdChannel for writing. */
		ClientServerChannel* outChannel;

		/** Simulation start time. */
		simtime_t startTime;

		/** Simulation stop time. */
		simtime_t stopTime;

		/** Current simulation time of vsimrti. */
		simtime_t currSimTime;

		/** Handle to thread, which waits for vsimrti time advance grant. */
		pthread_t receiveInteractionsThread;

		/** Mutex for cmdChannel locking and time advance waiting */
		pthread_mutex_t waitMutex;

		/** Condition var for time advance waiting */
		pthread_cond_t waitCond;

		/** Establishment of socket connection to VSimRTI via OMNeT++ Ambassador. */
		virtual void connectToAmbassador();

		/** Closes socket connections. */
		virtual void disconnectFromAmbassador();

		/** Reporting method, for next event to be allowed to schedule. */
		virtual void reportNextEvent(simtime_t nextSimTime);

		/** Wait method for time advance grant. */
		static void* receiveInteractions(void* p);
};

#endif /* VSIMRTIEVENTSCHEDULER_H_ */
