#pragma once

#include <WinSock2.h>
#include <string>
#include "NetworkMessage.h"

namespace df {

	class NetworkSocket {
	private:
		SOCKET m_sock;

		int receive(char* data, uint16_t dataSize, bool peek = false);

	public:
		int setNonBlocking();
		int setBlocking();

		NetworkSocket();
		~NetworkSocket();
		bool isConnected();

		int create();
		int bind(u_short port);
		int listen();
		int accept(NetworkSocket* newConnection) const;

		int connect(ULONG address, u_short port);

		int send(const NetworkMessage& message);
		int receive(NetworkMessage& message);

		int close();
	};

}