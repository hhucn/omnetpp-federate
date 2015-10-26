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

#ifndef __CLIENTSERVERCHANNEL_H__
#define __CLIENTSERVERCHANNEL_H__

//#define WINDOWS


#include <fstream>
#include <vector>
#include <map>
#include <stdexcept>

#ifdef WINDOWS

#include <winsock.h>
#define SocketErrno (WSAGetLastError())

#else

#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/select.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#define SocketErrno errno

#define SOCKET int
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

#endif



#include <errno.h>
#include <omnetpp.h>
#include "Coord.h"
#include "IPAddress.h"


using namespace std;


/**
 * Abstraction of sock communication between OMNeT++ Ambassador and Federate.
 *
 * @author rpr
 */
class ClientServerChannel {

	public:
		/** Constructor. */
		ClientServerChannel();

		/** Destructor. */
		~ClientServerChannel();

		/** Prepares connection with a socket bound to the given port on host. */
		virtual int			prepareConnection(string host, int port);

		/** Awaits connection on server socket. */
		virtual void		connect();

		/** Byte protocol control method for readCommand. */
		virtual int			readCommand();

		/** Byte protocol control method for readTime. */
		virtual simtime_t	readTime();

		/** Byte protocol control method for readId. */
		virtual int			readId();

		/** Byte protocol control method for readMessageId. */
		virtual int			readMessageId();

		/** Byte protocol control method for readCoordinates. */
		virtual Coord		readCoordinates();

		/** Byte protocol control method for readProtocol. */
		virtual int			readProtocol(void);

		/** Byte protocol control method for readChannelId. */
		virtual int			readChannelId(void);

		/** Byte protocol control method for readV2xMessageLength. */
		virtual int			readV2xMessageLength(void);

		/** Byte protocol control method for readIpAddress. */
		virtual IPAddress 	readIpAddress();

		/** Byte protocol control method for readRoutingTtl. */
		virtual int			readRoutingTtl(void);

		/** Byte protocol control method for readRoutingRadius. */
		virtual float		readRoutingRadius(void);

		/** Read the address type marker. */
		virtual int			readAddressMarker(void);

		/** Byte protocol control method for writeStatus. */
		virtual void		writeStatus(bool stat, string statMsg);

		/** Byte protocol control method for writeCommand. */
		virtual void		writeCommand(int cmd);

		/** Byte protocol control method for writeTime. */
		virtual void		writeTime(simtime_t time);

		/** Byte protocol control method for writeNodeId. */
		virtual void		writeNodeId(int id);

		/** Byte protocol control method for writeMessageId. */
		virtual void		writeMessageId(int id);

		/** Byte protocol control method for writeMessageReceiveSignalStrength. */
		virtual void		writeMessageReceiveSignalStrength(float rss);

	private:
		/** Initial server sock, which accepts connection of Ambassador. */
		SOCKET servsock;

		/** Working sock for communication. */
		SOCKET sock;

		/** Basic communication method for readInt. */
		virtual int			readInt(void);

		/** Basic communication method for readInt. */
		virtual char		readChar(void);

		/** Basic communication method for readLong. */
		virtual unsigned long long	readLong(void);

		/** Basic communication method for readFloat. */
		virtual float 		readFloat(void);

		/** Basic communication method for readBoolean. */
		virtual bool		readBoolean(void);

		/** Basic communication method for writeInt. */
		virtual void		writeInt(int val);

		/** Basic communication method for writeInt. */
		virtual void		writeChar(char val);

		/** Basic communication method for writeLong. */
		virtual void		writeLong(long long val);

		/** Basic communication method for writeFloat. */
		virtual void		writeFloat(float val);

		/** Basic communication method for writeBoolean. */
		virtual void		writeBoolean(bool val);

//		/** Basic communication method for writeString. */
//		void		writeString(string str);
};

#endif
