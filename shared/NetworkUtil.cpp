
#include "NetworkUtil.h"

#include <WinSock2.h>
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
