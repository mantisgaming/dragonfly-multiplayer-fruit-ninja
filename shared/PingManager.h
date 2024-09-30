#pragma once

#include <Object.h>
#include <EventStep.h>
#include "EventNetwork.h"

const int PING_INTERVAL = 15;

class PingManager : df::Object {
private:
	#ifdef CLIENT
	int m_timer;
	#endif
	int stepHandler(const df::EventStep* p_e);
	int networkHandler(const df::EventNetwork* p_e);
public:
	PingManager();
	int eventHandler(const df::Event* p_e) override;

};

