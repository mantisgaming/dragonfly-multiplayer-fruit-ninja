#pragma once

#include <stdint.h>

struct NetworkMessage
{
public:
	enum Type : uint8_t {
		UNDEFINED = -1
	};

	Type type;
	char* data;
	uint16_t dataSize;

	NetworkMessage();
	~NetworkMessage();
};

