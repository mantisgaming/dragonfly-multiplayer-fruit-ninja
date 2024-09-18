#include "EventNetwork.h"

namespace df {
	EventNetwork::EventNetwork(NetworkSocket* socket, Label label, char* data, uint16_t bytes) {
		m_socket = socket;
		m_label = label;
		m_data = data;
		m_bytes = bytes;
		setType(NETWORK_EVENT);
	}
}
