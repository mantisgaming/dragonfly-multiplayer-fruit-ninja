#include "NetworkObject.h"

#include <sstream>
#include <LogManager.h>
#include <WorldManager.h>

#include "NetworkMessage.h"
#include "NetworkManager.h"

NetworkObject** NetworkObject::objectList = new NetworkObject*[UINT8_MAX];

int NetworkObject::stepHandler(const df::EventStep* p_e) {
	if (m_counter == 0 && m_ticksPerSync > 0) {
		m_counter = m_ticksPerSync;
		synchronize();
	}

	m_counter--;
	return 1;
}

NetworkObject::NetworkObject(uint8_t typeID, uint8_t ticksPerSync, uint8_t networkID) {

	if (networkID == UINT8_MAX) {
		networkID = getFirstAvailableID();
	}

	if (networkID == UINT8_MAX) {
		LM.writeLog("ERROR: All network object indices are in use");
	}
	else {
		objectList[networkID] = this;
	}

	m_typeID = typeID;
	m_networkID = networkID;
	m_ticksPerSync = ticksPerSync;
	m_counter = ticksPerSync;

	if (ticksPerSync > 0)
		registerInterest(df::STEP_EVENT);
	
#ifdef CLIENT
	registerInterest(df::NETWORK_EVENT);
#endif
}

NetworkObject::~NetworkObject() {
	objectList[m_networkID] = NULL;

#ifdef SERVER
	syncDestroy();
#endif
}

int NetworkObject::subEventHandler(const df::Event* p_e)
{
	return 0;
}

NetworkObject* NetworkObject::getObject(uint8_t netID) {
	if (netID == UINT8_MAX)
		return NULL;
	return objectList[netID];
}

int NetworkObject::eventHandler(const df::Event* p_e) {
	int used = subEventHandler(p_e);

	if (p_e->getType() == df::STEP_EVENT)
		used |= stepHandler((df::EventStep*)p_e);

	return used;
}

uint8_t NetworkObject::getFirstAvailableID() {
	for (uint8_t i = 0; i < UINT8_MAX; i++) {
		if (objectList[i] == NULL)
			return i;
	}
	return UINT8_MAX;
}

void NetworkObject::synchronize(unsigned int attr, df::NetworkSocket* sock) {
#ifdef SERVER
	if (m_networkID == 0) {
		LM.writeLog("NetworkObject::synchronize(): WARNING: Attempted to synchronize object with invalid network ID");
		return;
	}

	NetworkMessage syncMsg;
	syncMsg.type = NetworkMessage::SYNC;

	std::stringstream stream;
	stream.write(reinterpret_cast<char*>(&m_networkID), 1);
	stream.write(reinterpret_cast<char*>(&m_typeID), 1);
	serialize(&stream, attr);
	
	std::string result = stream.rdbuf()->str();

	syncMsg.dataSize = (uint16_t) result.length();
	syncMsg.data = result.c_str();

	if (sock == NULL)
		NM.sendToAll(syncMsg);
	else
		sock->send(syncMsg);
#endif
}

void NetworkObject::syncDestroy() {
#ifdef SERVER
	NetworkMessage delMsg;
	delMsg.type = NetworkMessage::DESTROY;
	delMsg.data = new char(m_networkID);
	delMsg.dataSize = 1;

	NM.sendToAll(delMsg);

	delete delMsg.data;
#endif
}