#include "EventNetwork.h"

namespace df {
	EventNetwork::EventNetwork(NetworkSocket* socket, Label label, NetworkMessage* message) {
		m_socket = socket;
		m_label = label;
		m_message = message;
		setType(NETWORK_EVENT);
	}
}
