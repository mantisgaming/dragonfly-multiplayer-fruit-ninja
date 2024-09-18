#include "NetworkSocket.h"

#include <LogManager.h>

#include "NetworkUtil.h"

namespace df {

	int NetworkSocket::setNonBlocking() {
		u_long arg = 1;
		if (ioctlsocket(m_sock, FIONBIO, &arg)) {
			logNetworkError("ioctlsocket() FIONBIO Failed");
			return -1;
		}
		return 0;
	}

	NetworkSocket::NetworkSocket() {
		m_status = NSS_NEW;

		m_sock = ::socket(AF_INET, SOCK_STREAM, 0);

		if (m_sock < 0) {
			logNetworkError("Failed to create a new socket.");
			m_status = NSS_FAILED;
		}
	}

	int NetworkSocket::connect(IN_ADDR address, u_short port) {

		if (m_status != NSS_NEW) {
			LM.writeLog("ERROR: Only new sockets can be connected to a server.");
			return -1;
		}

		

		return 0;
	}

	int NetworkSocket::bind(u_short port) {

		if (m_status != NSS_NEW) {
			LM.writeLog("ERROR: Only new sockets can be bound to a port.");
			return -1;
		}

		SOCKADDR_IN serv_addr;

		/* Bind our local address so client can connect to us. */
		memset((void*)&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(port);
		if (::bind(m_sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
			logNetworkError("bind() failed");
			return -1;
		}

		m_status = NSS_BOUND;
		return 0;
	}

	int NetworkSocket::listen() {
		if (m_status != NSS_BOUND) {
			LM.writeLog("ERROR: Socket must be bound before it can begin listening.");
			return -1;
		}

		if (::listen(m_sock, 5) == -1) {
			logNetworkError("listen() failed");
			return -1;
		}

		m_status = NSS_LISTENING;
		return 0;
	}

	int NetworkSocket::accept(NetworkSocket* newConnection) {

		if (m_status != NSS_LISTENING) {
			LM.writeLog("WARNING: Socket must be listening to accept connections.");
			return -1;
		}



		return 0;
	}

}