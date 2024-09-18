#pragma once

#include <Event.h>
#include "NetworkSocket.h"

namespace df {

	const std::string NETWORK_EVENT = "df-network";

	class EventNetwork : public Event
	{
	public:
		enum Label : uint8_t {
			UNDEFINED = -1,
			ACCEPT,
			CONNECT,
			CLOSE,
			DATA,
		};

	private:
		Label m_label;
		NetworkSocket* m_socket;
		uint16_t m_bytes;
		char* m_data;

	public:
		EventNetwork(NetworkSocket* socket = NULL, Label label = UNDEFINED, char* data = NULL, uint16_t bytes = 0);

		inline Label getLabel() { return m_label; };
		inline NetworkSocket* getSocket() { return m_socket; };
		inline uint16_t getBytes() { return m_bytes; };
		inline char* getData() { return m_data; };
	};

}