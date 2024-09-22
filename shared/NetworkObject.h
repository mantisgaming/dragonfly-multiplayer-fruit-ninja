#pragma once

#include <Object.h>
#include <stdint.h>
#include <EventStep.h>
#include <vector>

#include "EventNetwork.h"
#include "NetworkSocket.h"

class NetworkObject : public df::Object {
private:
	uint8_t m_networkID;
	uint8_t m_typeID;
	uint8_t m_ticksPerSync; // 0 = don't sync
	uint8_t m_counter;

	int stepHandler(const df::EventStep* p_e);
	int eventHandler(const df::Event* p_e) override;
	static NetworkObject** objectList;
	static uint8_t getFirstAvailableID();

public:
	NetworkObject(uint8_t typeID, uint8_t ticksPerSync = 0, uint8_t networkID = UINT8_MAX);
	~NetworkObject();
	virtual int subEventHandler(const df::Event* p_e);
	constexpr uint8_t getNetworkID() const { return m_networkID; };
	static NetworkObject* getObject(uint8_t netID);
	static int registerObject(NetworkObject* obj);
	void alocateObject();
	void setNetID(uint8_t ID);
	void syncDestroy();
	void synchronize(unsigned int attr = 0, df::NetworkSocket* sock = NULL);
};

