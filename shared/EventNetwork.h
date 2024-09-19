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
		NetworkMessage* m_message;

	public:
		EventNetwork(NetworkSocket* socket = NULL, Label label = UNDEFINED, NetworkMessage* message);

		inline Label getLabel() { return m_label; };
		inline NetworkSocket* getSocket() { return m_socket; };
		inline const NetworkMessage* getMessage() { return m_message; };
	};

}