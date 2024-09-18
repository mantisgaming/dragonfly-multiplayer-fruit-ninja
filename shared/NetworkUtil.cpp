
#include "NetworkUtil.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <LogManager.h>

// write a network error to the log file
void logNetworkError(const char* message) {
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

// query DNS for address
int getAddress(std::string hostname, uint64_t* address) {
	ADDRINFOA hints;
	PADDRINFOA result;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = PF_INET;
	hints.ai_flags |= AI_CANONNAME;
	hints.ai_socktype = SOCK_STREAM;

	// query DNS for an IPv4 address
	if (getaddrinfo(hostname.c_str(), NULL, &hints, &result)) {
		logNetworkError("getaddrinfo() failed");
		return -1;
	}

	*address = ((sockaddr_in*)result)->sin_addr.S_un.S_addr;

	return 0;
}