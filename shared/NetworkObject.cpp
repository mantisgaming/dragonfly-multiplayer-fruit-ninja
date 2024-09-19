#include "NetworkObject.h"
#include "NetworkMessage.h"
#include <sstream>
#include <LogManager.h>

std::vector<uint8_t>* NetworkObject::freeIDs = NULL;

int NetworkObject::stepHandler(const df::EventStep* p_e) {
	if (m_counter == 0 && m_ticksPerSync > 0) {
		m_counter = m_ticksPerSync;
		synchronize();
	}

	m_counter --;
	return 1;
}

NetworkObject::NetworkObject(uint8_t networkID, uint8_t ticksPerSync) : m_networkID(networkID), m_ticksPerSync(ticksPerSync) {
	m_counter = m_ticksPerSync;
	if (ticksPerSync > 0)
		registerInterest(df::STEP_EVENT);
	
	synchronize();
}

int NetworkObject::eventHandler(const df::Event* p_e) {
	return 0;
}

void NetworkObject::synchronize(unsigned int attr) {
	NetworkMessage syncMsg;
	syncMsg.m_type

	std::stringstream stream;
	serialize(&stream, attr);
	
	std::string result = stream.rdbuf()->str();

	if (result.length() > sizeof(syncMsg.m_syncData)) {
		LM.writeLog("Attemtped to synchronize object with too much data");
		return;
	}

	memcpy(&syncMsg.m_syncData, result.c_str(), result.length());
	syncMsg.m_size = result.length();

}

uint8_t NetworkObject::getUniqueID() {
	
	if (freeIDs == NULL) {
		freeIDs = new std::vector<uint8_t>();
		for (int i = 0; i <= UINT8_MAX; i++) {
			freeIDs->push_back(i);
		}
	}
	if (freeIDs->size() == 0)
		return 0xffui8;
	
	return (*freeIDs)[0];
}
