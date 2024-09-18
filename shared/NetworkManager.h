#pragma once

#include <vector>
#include <stdint.h>

#include "NetworkSocket.h"

#define NM df::NetworkManager::getInstance()

#define DEFAULT_PORT 9876

namespace df {

	class NetworkManager {

	private:
		NetworkSocket* m_listener;
		std::vector<NetworkSocket*> m_connections;
		static inline NetworkManager* instance = NULL;

		NetworkManager();
		NetworkManager(NetworkManager& other);
		NetworkManager& operator=(NetworkManager& other);

	public:
		inline static NetworkManager& getInstance() {
			static NetworkManager nm;
			return nm;
		}

		int startupServer(uint16_t port);
		int shutdownServer();
		inline bool isServer() { return m_listener != NULL; }

		void accept();
		void sendToAll(char* data, int dataSize);
		void recieve();

		void closeAll();
		void checkConnections();
		bool hasConnection(NetworkSocket* sock);
		int getConnectionCount();
		int getConnections(NetworkSocket**& sockets);

		int startUp();
		void shutDown();
	};

}