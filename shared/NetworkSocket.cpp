#include "NetworkSocket.h"

#include <LogManager.h>
#include <sstream>

#include "NetworkUtil.h"
#include "EventNetwork.h"
#include "WorldManager.h"

// guard clause macros
#define REQUIRE_SOCKET				\
	if (m_sock == UINT64_MAX) {		\
		return -1;					\
	}

#define REQUIRE_NO_SOCKET			\
	if (m_sock != UINT64_MAX) {		\
		return -1;					\
	}

const char hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

namespace df {

	int NetworkSocket::setNonBlocking() {
		u_long arg = 1;
		if (ioctlsocket(m_sock, FIONBIO, &arg)) {
			logNetworkError("WARNING: ioctlsocket() FIONBIO Failed");
			return -1;
		}
		return 0;
	}

	NetworkSocket::NetworkSocket() {
		m_sock = -1;
	}

	NetworkSocket::~NetworkSocket()
	{
		close();
	}

	bool NetworkSocket::isConnected()
	{
		char data;
		return receive(&data, 1, true) >= 0;
	}

	int NetworkSocket::create() {
		REQUIRE_NO_SOCKET;

		m_sock = ::socket(AF_INET, SOCK_STREAM, 0);

		if (m_sock < 0) {
			logNetworkError("WARNING: socket() failed");
			m_sock = -1;
			return -1;
		}

		return 0;
	}

	int NetworkSocket::connect(ULONG address, u_short port) {
		REQUIRE_SOCKET;

		sockaddr_in info;
		memset(&info, 0, sizeof(info));

		info.sin_family = AF_INET;
		info.sin_addr.S_un.S_addr = htonl(address);
		info.sin_port = htons(port);

		if (::connect(m_sock, (sockaddr*)&info, sizeof(info))) {
			logNetworkError("WARNING: connect() failed");
			return -1;
		}

		setNonBlocking();

		return 0;
	}

	int NetworkSocket::send(NetworkMessage& message) {
		REQUIRE_SOCKET;

		std::stringstream stream;

		uint16_t totalDataSize = sizeof(totalDataSize) + sizeof(message.type) + message.dataSize;

		stream.write(reinterpret_cast<char*>(&totalDataSize), sizeof(totalDataSize));
		stream.write(reinterpret_cast<char*>(&message.type), sizeof(message.type));

		if (message.dataSize > 0)
			stream.write(message.data, message.dataSize);

		if (::send(m_sock, stream.str().c_str(), totalDataSize, 0) < 0) {
			switch (WSAGetLastError()) {
			case WSAEWOULDBLOCK:
				return 0;
			case WSAECONNABORTED:
			case WSAECONNRESET:
				LM.writeLog("INFO: A connection was reset");
				{
					EventNetwork e = EventNetwork(this, EventNetwork::Label::CLOSE);
					WM.onEvent(&e);
				}
				close();
				return -1;
			default:
				logNetworkError("WARNING: send() failed");
				return -1;
			}
		}

		std::string mystr = "";

		switch (message.type) {
		case NetworkMessage::UNDEFINED: mystr = "Undefined: "; break;
		case NetworkMessage::DESTROY: mystr = "Destroy: "; break;
		case NetworkMessage::DISCONNECT: mystr = "Disconnect: "; break;
		case NetworkMessage::SYNC: mystr = "Sync: "; break;
		case NetworkMessage::ASSIGN_CLIENT: mystr = "Assign Client: "; break;
		}

		for (int i = 0; i < message.dataSize; ++i) {
			mystr += "0x";
			mystr += hex[(message.data[i] & 0xf0) >> 4];
			mystr += hex[(message.data[i] & 0x0f)];
			mystr += " ";
		}
		LM.writeLog("INFO: Sent data packet: %s", mystr.c_str());

		return 0;
	}

	int NetworkSocket::receive(char* data, uint16_t dataSize, bool peek) {
		REQUIRE_SOCKET;

		int responseSize = ::recv(m_sock, data, dataSize, peek ? MSG_PEEK : 0);

		if (responseSize < 0) {
			
			switch (WSAGetLastError()) {
			case WSAEWOULDBLOCK:
				return 0;
			case WSAECONNABORTED:
			case WSAECONNRESET:
				LM.writeLog("INFO: A connection was reset");
				{
					EventNetwork e = EventNetwork(this, EventNetwork::Label::CLOSE);
					WM.onEvent(&e);
				}
				close();
				return -1;
			default:
				logNetworkError("WARNING: recv() failed");
				return -1;
			}
		}

		return responseSize;
	}

	int NetworkSocket::receive(NetworkMessage& message)
	{
		uint16_t totalSize;

		// check for full data header
		if (receive(reinterpret_cast<char*>(&totalSize), sizeof(totalSize), true) < sizeof(totalSize)) {
			return 0;
		}

		int dataSize = totalSize - sizeof(totalSize) - sizeof(message.type);
		char* buff = new char[totalSize];

		// check to make sure all data was received
		if (receive(buff, totalSize, true) < totalSize) {
			return 0;
		}

		// read data without peek to clear it from the stream
		if (receive(buff, totalSize, false) < totalSize) {
			return -1; // this should never happen
		}

		if (message.data != NULL)
			delete[] message.data;

		message.dataSize = dataSize;
		message.type = *reinterpret_cast<NetworkMessage::Type*>(buff + sizeof(message.dataSize));
		message.data = NULL;

		if (message.dataSize > 0) {
			message.data = new char[message.dataSize];
			memcpy((void*)message.data, buff + sizeof(message.dataSize) + sizeof(message.type), message.dataSize);
		}

		delete[] buff;

		std::string mystr = "";

		switch (message.type) {
		case NetworkMessage::UNDEFINED: mystr = "Undefined: "; break;
		case NetworkMessage::DESTROY: mystr = "Destroy: "; break;
		case NetworkMessage::DISCONNECT: mystr = "Disconnect: "; break;
		case NetworkMessage::SYNC: mystr = "Sync: "; break;
		case NetworkMessage::ASSIGN_CLIENT: mystr = "Assign Client: "; break;
		}

		for (int i = 0; i < message.dataSize; ++i) {
			mystr += "0x";
			mystr += hex[(message.data[i] & 0xf0) >> 4];
			mystr += hex[(message.data[i] & 0x0f)];
			mystr += " ";
		}
		LM.writeLog("INFO: Recieved data packet: %s", mystr.c_str());

		return dataSize;
	}

	int NetworkSocket::close() {
		REQUIRE_SOCKET;

		if (closesocket(m_sock)) {
			logNetworkError("WARNING: closesocket() failed");
			return -1;
		}

		m_sock = -1;
		return 0;
	}

	int NetworkSocket::bind(u_short port) {
		REQUIRE_SOCKET;

		sockaddr_in info;
		memset((void*)&info, 0, sizeof(info));

		info.sin_family = AF_INET;
		info.sin_addr.s_addr = htonl(INADDR_ANY);
		info.sin_port = htons(port);

		if (::bind(m_sock, (sockaddr*)&info, sizeof(info)) < 0) {
			logNetworkError("WARNING: bind() failed");
			return -1;
		}

		return 0;
	}

	int NetworkSocket::listen() {
		REQUIRE_SOCKET;

		if (::listen(m_sock, 5) == -1) {
			logNetworkError("WARNING: listen() failed");
			return -1;
		}

		setNonBlocking();

		return 0;
	}

	int NetworkSocket::accept(NetworkSocket* newConnection) const {
		REQUIRE_SOCKET;

		sockaddr_in info;
		int infoSize = (int)sizeof(info);

		newConnection->close();

		SOCKET newSock = ::accept(m_sock, (sockaddr*)&info, &infoSize);

		if (newSock < 0) {
			switch (WSAGetLastError()) {
			case WSAEWOULDBLOCK:
				return 0;
			default:
				logNetworkError("WARNING: accept() failed");
				return -1;
			}
		}
		newConnection->m_sock = newSock;
		return 0;
	}

}