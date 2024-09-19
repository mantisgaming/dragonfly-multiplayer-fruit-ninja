#pragma once

#include <stdint.h>

#define NMT_UNDEFINED -1

class NetworkMessage
{
protected:
	inline NetworkMessage(uint8_t messageType = NMT_UNDEFINED) : m_type(messageType) { };
private:
	uint8_t m_type;
public:
	inline uint8_t getType() { return m_type; };
};

