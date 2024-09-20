#include "NetworkObject.h"
#include "NetworkMessage.h"
#include "NetworkManager.h"
#include <sstream>
#include <LogManager.h>

std::vector<uint8_t>* NetworkObject::freeIDs = NULL;

int NetworkObject::stepHandler(const df::EventStep* p_e) {
	if (m_counter == 0 && m_ticksPerSync > 0) {
		m_counter = m_ticksPerSync;
		synchronize();
	}

	m_counter--;
	return 1;
}

NetworkObject::NetworkObject(uint8_t networkID, uint8_t ticksPerSync) : m_networkID(networkID), m_ticksPerSync(ticksPerSync) {
	m_counter = m_ticksPerSync;
	if (ticksPerSync > 0)
		registerInterest(df::STEP_EVENT);
	
	synchronize();
}

NetworkObject::~NetworkObject() {
	freeIDs->push_back(m_networkID);
	if (m_ticksPerSync > 0)
		syncDestroy();
}

int NetworkObject::subEventHandler(const df::Event* p_e)
{
	return 0;
}

int NetworkObject::eventHandler(const df::Event* p_e) {
	int used = subEventHandler(p_e);

	if (p_e->getType() == df::STEP_EVENT)
		used |= stepHandler((df::EventStep*)p_e);

	return used;
}

void NetworkObject::synchronize(unsigned int attr) {
	if (m_networkID == 0) {
		LM.writeLog("NetworkObject::synchronize(): WARNING: Attempted to synchronize object with invalid network ID");
		return;
	}

	NetworkMessage syncMsg;
	syncMsg.type = NetworkMessage::SYNC;

	std::stringstream stream;
	serialize(&stream, attr);
	
	std::string result = stream.rdbuf()->str();

	syncMsg.dataSize = (uint16_t) result.length();
	syncMsg.data = result.c_str();

	NM.sendToAll(syncMsg);
}

void NetworkObject::syncDestroy() {
	NetworkMessage delMsg;
	delMsg.type = NetworkMessage::DESTROY;
	delMsg.data = new char(m_networkID);
	delMsg.dataSize = 1;

	NM.sendToAll(delMsg);

	delete delMsg.data;
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

	uint8_t id = (*freeIDs)[freeIDs->size()];
	
	freeIDs->pop_back();

	return id;
}
