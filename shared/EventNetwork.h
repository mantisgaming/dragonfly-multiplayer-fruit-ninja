#pragma once

#include <Event.h>
#include "NetworkSocket.h"

namespace df {

	const std::string NETWORK_EVENT = "df-network";

	class EventNetwork : public Event
	{
	public:
		enum Label : uint8_t {
			UNDEFINED = 0xffui8,
			ACCEPT = 0,
			CONNECT,
			CLOSE,
			DATA,
		};

	private:
		Label m_label;
		NetworkSocket* m_socket;
		NetworkMessage* m_message;

	public:
		EventNetwork(NetworkSocket* socket = NULL, Label label = UNDEFINED, NetworkMessage* message = NULL);

		inline Label getLabel() const { return m_label; };
		inline NetworkSocket* getSocket() const { return m_socket; };
		inline const NetworkMessage* getMessage() const { return m_message; };
	};

}