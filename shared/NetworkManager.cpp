#include "NetworkManager.h"

#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <io.h>

#include <LogManager.h>


namespace df {

	NetworkManager::NetworkManager() {
		listener = NULL;
		connections.clear();
	}
	
	NetworkManager::NetworkManager(NetworkManager& other) : NetworkManager() {}

	NetworkManager& NetworkManager::operator=(NetworkManager& other) { return other; }

	int NetworkManager::initializeServer() {
		if (isServer()) return NME_NOTHING_TO_DO;

		//listener = new NetworkSocket(NSM_LISTEN);

		return NME_SUCCESS;
	}

	int NetworkManager::shutdownServer() {
		if (!isServer()) return NME_NOTHING_TO_DO;

		delete listener;
		listener = NULL;

		return NME_SUCCESS;
	}

	// write a network error to the log file
	void print_network_error(const char* message) {
		LPTSTR error_text = NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM
			| FORMAT_MESSAGE_ALLOCATE_BUFFER
			| FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&error_text,  /* Output. */
			0, /* Min. size for output buffer. */
			NULL);
		if (error_text != NULL) {
			LM.writeLog("%s: %d - %S", message, WSAGetLastError(), error_text);
			LocalFree(error_text);
			error_text = NULL;
		}
	}

	int NetworkManager::setServer(bool server) {

		// if already in the correct mode, do nothing
		if (isServer() == server) return NME_NOTHING_TO_DO;

		if (server) {
			return initializeServer();
		} else {
			return shutdownServer();
		}
	}

	int NetworkManager::startUp() {

		// initialize WSA
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR) {
			print_network_error("WSAStartup() failed");
			return NME_WSASTARTUP_FAILED;
		}

		return NME_SUCCESS;
	}

	void NetworkManager::shutDown() {

		// shutdown WSA
		if (WSACleanup() != NO_ERROR) {
			print_network_error("WSACleanup() failed");
		}

	}

}