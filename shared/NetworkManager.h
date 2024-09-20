#pragma once

#include <vector>
#include <stdint.h>

#include "NetworkSocket.h"

#define NM df::NetworkManager::getInstance()

#define DEFAULT_PORT 9876

#define SERVER_ONLY(code) if (NM.isServer()) {code}
#define CLIENT_ONLY(code) if (NM.isClient()) {code}

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