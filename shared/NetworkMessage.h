#pragma once

#include <stdint.h>

struct NetworkMessage
{
public:
	enum Type : uint8_t {
		UNDEFINED = 0xffui8,
		SYNC = 0,
		DESTROY,
		DISCONNECT,
		ASSIGN_CLIENT,
		SWORD_POSITION,
	};

	Type type;
	const char* data;
	uint16_t dataSize;
};