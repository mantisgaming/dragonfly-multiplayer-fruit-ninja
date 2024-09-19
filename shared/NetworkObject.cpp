#include "NetworkObject.h"

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

void NetworkObject::synchronize() {
	// TODO synchronize
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
