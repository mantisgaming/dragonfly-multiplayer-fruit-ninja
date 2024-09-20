#pragma once

#include <vector>
#include <stdint.h>

#include "NetworkSocket.h"

#define NM df::NetworkManager::getInstance()

#define DEFAULT_PORT 9876

#define SERVER_ONLYR(retCode) if (NM.isClient()) return retCode
#define SERVER_ONLYV if (NM.isClient()) return
#define CLIENT_ONLYR(retCode) if (NM.isServer()) return retCode
#define CLIENT_ONLYV if (NM.isServer()) return

namespace df {

	class NetworkManager {

	private:
		NetworkSocket* m_listener;
		std::vector<NetworkSocket*> m_connections;
		static inline NetworkManager* instance = NULL;

		NetworkManager();
		NetworkManager(NetworkManager& other);
		NetworkManager& operator=(NetworkManager& other);

		void recieve(NetworkSocket* sock);

	public:
		inline static NetworkManager& getInstance() {
			static NetworkManager nm;
			return nm;
		}

		int startupServer(uint16_t port);
		int shutdownServer();
		constexpr bool isServer() const { return m_listener != NULL; }
		constexpr bool isClient() const { return m_listener == NULL; }

		void accept();
		void sendToAll(NetworkMessage& message);
		void recieve();
		int connect(std::string address, uint16_t port);

		void closeAll();
		void checkConnections();
		bool hasConnection(NetworkSocket* sock);
		int getConnectionCount();
		int getConnections(NetworkSocket**& sockets);

		int startUp();
		void shutDown();
	};

}