#include "NetworkObject.h"

#include <sstream>
#include <LogManager.h>
#include <WorldManager.h>

#include "NetworkMessage.h"
#include "NetworkManager.h"

std::vector<uint8_t>* NetworkObject::freeIDs = NULL;

int NetworkObject::stepHandler(const df::EventStep* p_e) {
	if (m_counter == 0 && m_ticksPerSync > 0) {
		m_counter = m_ticksPerSync;
		synchronize();
	}

	m_counter--;
	return 1;
}

int NetworkObject::networkHandler(const df::EventNetwork* p_e) {

	if (p_e->getLabel() != df::EventNetwork::DATA) return 0;

	const NetworkMessage& message = *p_e->getMessage();
	if (message.type != NetworkMessage::SYNC && message.type != NetworkMessage::DESTROY) return 0;

	uint8_t netID;

	std::stringstream stream;
	stream.write(message.data, message.dataSize);
	stream.read(reinterpret_cast<char*>(&netID), 1);

	if (netID != m_networkID) return 0;
	
	switch (message.type) {
	case NetworkMessage::SYNC:
		deserialize(&stream);
		break;
	case NetworkMessage::DESTROY:
		WM.markForDelete(this);
		break;
	}

	return 1;
}

NetworkObject::NetworkObject(uint8_t typeID, uint8_t networkID, uint8_t ticksPerSync)
	: m_typeID(typeID), m_networkID(networkID), m_ticksPerSync(ticksPerSync) {
	m_counter = m_ticksPerSync;
	if (ticksPerSync > 0)
		registerInterest(df::STEP_EVENT);
	
	registerInterest(df::NETWORK_EVENT);
}

NetworkObject::~NetworkObject() {
#ifdef SERVER
	freeIDs->push_back(m_networkID);
	if (m_ticksPerSync > 0)
		syncDestroy();
#endif
}

int NetworkObject::subEventHandler(const df::Event* p_e)
{
	return 0;
}

int NetworkObject::eventHandler(const df::Event* p_e) {
	int used = subEventHandler(p_e);

	if (p_e->getType() == df::STEP_EVENT)
		used |= stepHandler((df::EventStep*)p_e);

	if (p_e->getType() == df::NETWORK_EVENT)
		used |= networkHandler((df::EventNetwork*)p_e);

	return used;
}

void NetworkObject::synchronize(unsigned int attr) {
#ifdef SERVER
	if (m_networkID == 0) {
		LM.writeLog("NetworkObject::synchronize(): WARNING: Attempted to synchronize object with invalid network ID");
		return;
	}

	NetworkMessage syncMsg;
	syncMsg.type = NetworkMessage::SYNC;

	std::stringstream stream;
	stream.write(reinterpret_cast<char*>(&m_networkID), 1);
	serialize(&stream, attr);
	
	std::string result = stream.rdbuf()->str();

	syncMsg.dataSize = (uint16_t) result.length();
	syncMsg.data = result.c_str();

	NM.sendToAll(syncMsg);
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

void NetworkObject::syncSpawn() {
#ifdef SERVER
	if (m_networkID == 0) {
		LM.writeLog("NetworkObject::synchronize(): WARNING: Attempted to spawn object with invalid network ID");
		return;
	}

	NetworkMessage spawnMsg;
	spawnMsg.type = NetworkMessage::SPAWN;

	std::stringstream stream;
	stream.write(reinterpret_cast<char*>(&m_networkID), 1);
	stream.write(reinterpret_cast<char*>(&m_typeID), 1);
	serialize(&stream, -1);

	std::string result = stream.rdbuf()->str();

	spawnMsg.dataSize = (uint16_t)result.length();
	spawnMsg.data = result.c_str();

	NM.sendToAll(spawnMsg);
#endif
}

uint8_t NetworkObject::getUniqueID() {
	
	if (freeIDs == NULL) {
		freeIDs = new std::vector<uint8_t>();
		for (int i = UINT8_MAX; i > 0; i--) {
			freeIDs->push_back(i);
		}
	}

	if (freeIDs->size() == 0)
		return 0xffui8;

	uint8_t id = (*freeIDs)[freeIDs->size() - 1];
	
	freeIDs->pop_back();

	return id;
}
