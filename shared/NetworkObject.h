#pragma once

#include<Object.h>
#include<stdint.h>
#include<EventStep.h>
#include<vector>

class NetworkObject : public df::Object {
private:
	uint8_t m_networkID;
	uint8_t m_ticksPerSync; // 0 = don't sync
	uint8_t m_counter;

	int stepHandler(const df::EventStep* p_e);
	static std::vector<uint8_t>* freeIDs;

public:
	NetworkObject(uint8_t networkID, uint8_t ticksPerSync = 0);
	int eventHandler(const df::Event* p_e) override;
	void synchronize(unsigned int attr = 0);
	inline uint8_t getNetworkID() { return m_networkID; };
	static uint8_t getUniqueID();
};

