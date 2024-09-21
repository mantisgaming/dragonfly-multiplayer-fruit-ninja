#pragma once

#include <Object.h>
#include <stdint.h>
#include <EventStep.h>
#include <vector>

#include "EventNetwork.h"

class NetworkObject : public df::Object {
private:
	uint8_t m_networkID;
	uint8_t m_typeID;
	uint8_t m_ticksPerSync; // 0 = don't sync
	uint8_t m_counter;

	int stepHandler(const df::EventStep* p_e);
	int networkHandler(const df::EventNetwork* p_e);
	static std::vector<uint8_t>* freeIDs;
	int eventHandler(const df::Event* p_e) override;
	void syncDestroy();
	void syncSpawn();

public:
	NetworkObject(uint8_t typeID, uint8_t networkID = 0, uint8_t ticksPerSync = 0);
	~NetworkObject();
	virtual int subEventHandler(const df::Event* p_e);
	void synchronize(unsigned int attr = 0);
	constexpr uint8_t getNetworkID() const { return m_networkID; };
	static uint8_t getUniqueID();
};

