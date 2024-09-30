#pragma once

#include <WinSock2.h>
#include <string>
#include <queue>
#include "NetworkMessage.h"

namespace df {

	class NetworkSocket {
	private:
		using DelayedMessage = std::tuple<int, char *, uint16_t>;

		SOCKET m_sock;
		int m_delay;
		std::queue<DelayedMessage> m_delayQueue;

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
		void setDelay(int delay);

		int connect(ULONG address, u_short port);

		int send(const NetworkMessage& message);
		int receive(NetworkMessage& message);

		int flush();

		int close();
	};

}