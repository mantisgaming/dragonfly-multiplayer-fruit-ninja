#pragma once

#include <WinSock2.h>
#include <string>

#define NSS_NEW 0
#define NSS_CREATED 1
#define NSS_FAILED -1
#define NSS_CONNECTED 2
#define NSS_BOUND 3
#define NSS_LISTENING 4
#define NSS_DISCONNECTED 5

namespace df {

	class NetworkSocket {
	private:
		SOCKET m_sock;
		int m_status;

		int setNonBlocking();

	public:
		NetworkSocket();
		inline SOCKET getDescriptor() const { return m_sock; };
		inline int getStatus() const { return m_status; };

		int connect(IN_ADDR address, u_short port);
		int bind(u_short port);
		int listen();

		int accept(NetworkSocket* newConnection);
	};

}