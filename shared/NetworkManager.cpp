#include "NetworkManager.h"
#include "NetworkUtil.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <io.h>

#include "EventNetwork.h"


namespace df {

	NetworkManager::NetworkManager() {
		m_listener = NULL;
		m_connections.clear();
	}
	
	NetworkManager::NetworkManager(NetworkManager& other) : NetworkManager() {}

	NetworkManager& NetworkManager::operator=(NetworkManager& other) { return other; }

	int NetworkManager::startupServer(uint16_t port) {
		if (isServer()) return 0;

		m_listener = new NetworkSocket();

		if (m_listener->bind(port)) {
			delete m_listener;
			m_listener = NULL;
			return -1;
		}

		if (m_listener->listen()) {
			delete m_listener;
			m_listener = NULL;
			return -1;
		}

		return 0;
	}

	int NetworkManager::shutdownServer() {
		if (!isServer()) return 0;

		delete m_listener;
		m_listener = NULL;

		return 0;
	}

	void NetworkManager::accept()
	{
		while (true) {
			NetworkSocket* sock = new NetworkSocket();
			// if the accept call fails or the socket is not connected, break the loop
			if (m_listener->accept(sock) || (!sock->isConnected())) {
				delete sock;
				return;
			}

			// TODO create accept event

			m_connections.push_back(sock);
		}
	}

	void NetworkManager::sendToAll(char* data, int dataSize) {
		for (int i = 0; i < m_connections.size(); i++) {
			m_connections[i]->send(data, dataSize);
		}
	}

	void NetworkManager::recieve() {
		for (int i = 0; i < m_connections.size(); i++) {
			char* data;
			int dataSize = m_connections[i]->receive(data);

			if (dataSize > 0) {
				// TODO create data event
			}
		}
	}

	void NetworkManager::closeAll()
	{
		for (int i = 0; i < m_connections.size(); i++) {
			// TODO create disconnect event

			delete (m_connections[i]);
		}

		m_connections.clear();
	}

	void NetworkManager::checkConnections()
	{
		for (int i = 0; i < m_connections.size(); i++) {
			if (!m_connections[i]->isConnected()) {
				// TODO create disconnect event

				delete (m_connections[i]);
				m_connections[i] = m_connections[m_connections.size() - 1];
				m_connections.pop_back();
				i--;
			}
		}
	}

	bool NetworkManager::hasConnection(NetworkSocket* sock) {
		for (int i = 0; i < m_connections.size(); i++) {
			if (m_connections[i] == sock)
				return true;
		}
		return false;
	}

	int NetworkManager::getConnectionCount() {
		return (int)m_connections.size();;
	}

	int NetworkManager::getConnections(NetworkSocket**& sockets) {

		int count = getConnectionCount();
		sockets = new NetworkSocket*[count];

		for (int i = 0; i < count; i++) {
			sockets[i] = m_connections[i];
		}

		return count;
	}

	int NetworkManager::startUp() {

		// initialize WSA
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR) {
			logNetworkError("WSAStartup() failed");
			return -1;
		}

		return 0;
	}

	void NetworkManager::shutDown() {

		// shutdown WSA
		if (WSACleanup() != NO_ERROR) {
			logNetworkError("WSACleanup() failed");
		}

	}

}