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

#include "ChannelCMD.h"
#include "ClientServerChannel.h"


/**
 * Constructor.
 */
ClientServerChannel::ClientServerChannel() {

	#ifdef WINDOWS
		WSADATA wsaData;

		int iResult;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed: %d\n", iResult);
		}
	#endif

	servsock = INVALID_SOCKET;
	sock = INVALID_SOCKET;
}

/**
 * Provides server socket for incoming messages from OMNeT++ Ambassador using given port on host.
 *
 * @param host
 * 		own hostname (hostaddress)
 * @param port
 *		port to listen on for incoming connections
 * @return
 * 		assigned port number
 */
int ClientServerChannel::prepareConnection(string host, int port) {

	in_addr addr;
	struct hostent* host_ent;
	struct in_addr saddr;

	saddr.s_addr = inet_addr(host.c_str());
	if (saddr.s_addr != static_cast<unsigned int>(-1)) {
		addr = saddr;
	} else if ((host_ent = gethostbyname(host.c_str()))) {
		addr = *((struct in_addr*)host_ent->h_addr_list[0]);
	} else {
		cerr << "Error: Invalid host address: " << host.c_str() << endl;
		return 0;
	}

	sockaddr_in servaddr;
	memset( (char*)&servaddr, 0, sizeof(servaddr) );
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = addr.s_addr;

	servsock = socket(AF_INET,SOCK_STREAM, 0 );
	if (servsock < 0)
		cerr << "Error: ClientServerChannel could not create socket to connect to Ambassador - " << strerror(errno) << endl;

	if (bind(servsock, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
		cerr << "Warn: ClientServerChannel could not bind socket to Ambassador - " << strerror(errno) << endl;

	listen(servsock, 3);

	int len = sizeof(servaddr);
	#ifdef WINDOWS
		getsockname(servsock, (struct sockaddr*) &servaddr, &len);//(socklen_t*)
	#else // LINUX
		getsockname(servsock, (struct sockaddr*) &servaddr,(socklen_t*) &len);
	#endif
	return ntohs(servaddr.sin_port);
}

/**
 * Connects OMNeT++ Federate to Ambassador.
 */
void ClientServerChannel::connect(void) {

	sockaddr_in address;
	int len = sizeof(address);
	#ifdef WINDOWS
		sock = accept(servsock, (struct sockaddr*) &address, &len);//(socklen_t*)
	#else // LINUX
		sock = accept(servsock, (struct sockaddr*) &address, (socklen_t*) &len);
	#endif

	if (sock < 0)
		cerr << "Error: ClientServerChannel could not accept connection from Ambassador - " << strerror(errno) << endl;

	int x = 1;
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&x, sizeof(x));
}

/**
 * Closes existing network connections.
 */
ClientServerChannel::~ClientServerChannel() {

	if (sock >= 0) {
		close(sock);
		sock = -1;
	}
	if (servsock >= 0) {
		close(servsock);
		servsock = -1;
	}
	#ifdef WINDOWS
		WSACleanup();
	#endif
}

/**
 * Gets command from OMNeT++ Ambassador to select dedicated action.
 *
 * @return
 * 		command from Ambassador
 */
int	ClientServerChannel::readCommand() {

	return readInt();
}

/**
 * Gets time from OMNeT++ Ambassador for scheduling of startTime, stopTime and timeAdvance.
 *
 * @return
 * 		time as simtime
 */
simtime_t ClientServerChannel::readTime() {

	unsigned long long lTime = readLong();
	simtime_t sTime;
	sTime.setRaw(lTime);
	return sTime;
}

/**
 * Gets id from OMNeT++ Ambassador.
 * Applicable for involved nodes (vehicle, rsu)
 *
 * @return
 * 		Id
 */
int ClientServerChannel::readId() {

	return readInt();
}

/**
 * Gets message id from OMNeT++ Ambassador.
 *
 * @return
 * 		Id
 */
int ClientServerChannel::readMessageId() {

	return readInt();
}

/**
 * Gets projected position coordinates from OMNeT++ Ambassador.
 *
 * @return
 * 		position coordinates (x,y)
 */
Coord ClientServerChannel::readCoordinates() {

	double posx = static_cast<double> (readFloat());
	double posy = static_cast<double> (readFloat());
	return new Coord(posx, posy);
}

/**
 * Gets application layer protocol of v2x message from OMNeT++ Ambassador.
 *
 * @return
 * 		AppProtocolType (tcp or udp)
 */
int ClientServerChannel::readProtocol() {

	return readInt();
}

/**
 * Gets the wlan channel
 */
int ClientServerChannel::readChannelId() {
    return readInt();
}

/**
 * Gets length of v2x message from OMNeT++ Ambassador.
 *
 * @return
 * 		msgLength
 */
int ClientServerChannel::readV2xMessageLength() {
	return readInt();
}

/**
 * Gets destination ip address of v2x message from OMNeT++ Ambassador.
 *
 * @return
 * 		ipAddress (IPv4)
 */
IPAddress ClientServerChannel::readIpAddress() {

	int ip[4] = {255, 255, 255, 255}; // IPv4
	for (int i = 0; i < 4; i++) {
		 ip[i] = readInt();
	}
	return IPAddress(ip[0], ip[1], ip[2], ip[3]);
}

/**
 * Gets hop count for topologically scoped v2x messages from OMNeT++ Ambassador.
 *
 * @return
 * 		ttl
 */
int ClientServerChannel::readRoutingTtl(void) {

	return readInt();
}

/**
 * Gets radius for geographically scoped v2x messages from OMNeT++ Ambassador.
 *
 * @return
 * 		radius
 */
float ClientServerChannel::readRoutingRadius(void) {

	return readFloat();
}

/**
 * Read the address type marker.
 *
 * @return
 * 		marker
 */
int ClientServerChannel::readAddressMarker(void) {

	return readInt();
}

/**
 * Informs OMNeT++ Ambassador about success of last interaction.
 *
 * @param stat
 * 		state of success (true, false)
 * @param statMsg
 * 		detailed description string - currently not evaluated
 */
void ClientServerChannel::writeStatus(bool stat, string statMsg) {

	writeBoolean(stat);	// 1st approach: only success and failure information
}

/**
 * Sends own control commands to vsimrti (esp for next_event and recv_message).
 *
 * @param cmd
 * 		command to be written to vsimrti
 */
void ClientServerChannel::writeCommand(int cmd) {

	writeInt(cmd);
}

/**
 * Writes time to vsimrti.
 *
 * @param time
 * 		time to be written to vsimrti
 */
void ClientServerChannel::writeTime(simtime_t time) {

	long long lTime = time.raw();
	writeLong(lTime);
}

/**
 * Writes node id to vsimrti.
 * Applicable for node ids.
 *
 * @param id
 * 		 node id
 */
void ClientServerChannel::writeNodeId(int id) {

	writeInt(id);
}

/**
 * Writes message id to vsimrti.
 * Applicable for message ids.
 *
 * @param id
 * 		msg id
 */
void ClientServerChannel::writeMessageId(int id) {

	writeInt(id);
}

/**
 * Writes message receive signal strength to vsimrti.
 * Applicable for message rss.
 *
 * @param rss
 * 		receive signal strength
 */
void ClientServerChannel::writeMessageReceiveSignalStrength(float rss) {

	writeFloat(rss);
}

/**
 * Provides read functionality for dedicated basic data type integer (32bit).
 */
int ClientServerChannel::readInt(void) {

	union {
		char c[4];
		int i;
	} buf;
	int rec = 0;

	for (unsigned int n = 0; n < sizeof(int); n++) {
		rec = recv(sock,  &buf.c[n], 1, 0);//(void *)
	}

	return ntohl(buf.i);
}

/**
 * Provides read functionality for dedicated basic data type char (8bit).
 */
char ClientServerChannel::readChar(void) {

	char c;
	int rec = 0;

	rec = recv(sock, &c, sizeof(char), 0);//(void*)
	if (rec != sizeof(char))
		cRuntimeError("readChar Error: %d", rec);

	return c;
}

/**
 * Provides read functionality for dedicated basic data type long (64bit).
 */
unsigned long long ClientServerChannel::readLong(void) {

	union {
		char c[8];
		int i[2];
	} buf;
	unsigned long long l;
	int rec = 0;

	for (unsigned int n = 0; n < sizeof(unsigned long long); n++) {
		rec = recv(sock, &buf.c[n], 1, 0);//(void *)
	}

	l = ((unsigned long long) ntohl(buf.i[0]) << 32) | ((unsigned long long) ntohl(buf.i[1]));
	return l;
}

/**
 * Provides read functionality for dedicated basic data type float (32bit).
 */
float ClientServerChannel::readFloat(void) {

	union {
		char c[4];
		int i;
		float f;
	} buf;
	int rec = 0;

	for (unsigned int n = 0; n < sizeof(float); n++) {
		rec = recv(sock, &buf.c[n], 1, 0);//(void *)
	}

	buf.i = ntohl(buf.i);
	return buf.f;
}

/**
 * Provides read functionality for dedicated basic data type boolean.
 */
bool ClientServerChannel::readBoolean(void) {

	bool val = false;
	if (readInt() == 1) {
		val = true;
	}
	return val;
}

/**
 * Provides write functionality for dedicated basic data type integer (32bit).
 */
void ClientServerChannel::writeInt(int val) {

	union {
		char c[4];
		int i;
	} buf;

	buf.i = htonl(val);
	send(sock, buf.c, sizeof(int), 0);
}

/**
 * Provides write functionality for dedicated basic data type char (8bit).
 */
void ClientServerChannel::writeChar(char val) {

	char c = val;
	send(sock, &c, sizeof(char), 0);//(void*)
}

/**
 * Provides write functionality for dedicated basic data type long (64bit).
 */
void ClientServerChannel::writeLong(long long val) {

	union {
		char c[8];
		int i[2];
	} buf;

	buf.i[0] = htonl(val >> 32);
	buf.i[1] = htonl(val & 0xFFFFFFFFLL);
	send(sock, buf.c, sizeof(long long), 0);
}

/**
 * Provides write functionality for dedicated basic data type float (32bit).
 */
void ClientServerChannel::writeFloat(float val) {

	union {
		char c[4];
		int i;
		float f;
	} buf;

	buf.f = val;
	buf.i = htonl(buf.i);
	send(sock, buf.c, sizeof(float), 0);
}

/**
 * Provides write functionality for dedicated basic data type boolean.
 */
void ClientServerChannel::writeBoolean(bool val) {

	if (val == true) {
		writeInt(1);
	}
	else {
		writeInt(0);
	}
}

///**
// * Provides write functionality for dedicated data type of string.
// */
//void ClientServerChannel::writeString(string str) {
//
//}
