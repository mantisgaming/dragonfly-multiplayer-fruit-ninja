#pragma once

#include <ViewObject.h>
#include <EventStep.h>

#include "NetworkObject.h"

const std::string TIMER_STRING = "Timer";
const uint8_t TIMER_TYPE_ID = 3;

class Timer : public NetworkObject, public df::ViewObject {
private:
    int subEventHandler(const df::Event* p_e);
	int step(const df::EventStep* p_e);
public:
	Timer();
	int serialize(std::stringstream* p_ss, unsigned int attr = 0) override;
	int deserialize(std::stringstream* p_ss, unsigned int* p_a = NULL) override;
};
