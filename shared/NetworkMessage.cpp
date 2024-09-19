#include "NetworkMessage.h"

NetworkMessage::~NetworkMessage() {
	if (data == NULL)
		delete[] data;
}
