#pragma once

#include <vector>
#include <Manager.h>

#include "NetworkSocket.h"

#define NME_SUCCESS 0
#define NME_NOTHING_TO_DO -1
#define NME_WSASTARTUP_FAILED -2

#define NM df::NetworkManager::getInstance()

namespace df {

	class NetworkManager : Manager {

	private:
		NetworkSocket* listener;
		std::vector<NetworkSocket*> connections;
		static inline NetworkManager* instance = NULL;

		NetworkManager();
		NetworkManager(NetworkManager& other);
		NetworkManager& operator=(NetworkManager& other);

		int initializeServer();
		int shutdownServer();

	public:
		inline static NetworkManager& getInstance() {
			static NetworkManager nm;
			return nm;
		}

		int setServer(bool server);
		inline bool isServer() { return listener != NULL; }

		int startUp() override;
		void shutDown() override;
	};

}