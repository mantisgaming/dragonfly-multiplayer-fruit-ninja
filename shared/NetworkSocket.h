#pragma once

#include <WinSock2.h>
#include <string>

namespace df {

	class NetworkSocket {
	private:
		SOCKET m_sock;

		int setNonBlocking();
		int receive(char* data, uint16_t dataSize, bool peek = false);

	public:
		NetworkSocket();
		~NetworkSocket();
		bool isConnected();

		int create();
		int bind(u_short port);
		int listen();
		int accept(NetworkSocket* newConnection) const;

		int connect(ULONG address, u_short port);

		int send(char* data, uint16_t dataSize);
		int receive(char*& data);

		int close();
	};

}