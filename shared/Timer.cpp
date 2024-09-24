#include "Timer.h"

#include <ResourceManager.h>

Timer::Timer() : NetworkObject(TIMER_TYPE_ID, 150) {
	df::Object::setType(TIMER_STRING);
	setLocation(df::TOP_LEFT);
	setViewString(TIMER_STRING);
	setColor(df::WHITE);
	df::Object::registerInterest(df::STEP_EVENT); // Count ticks as seconds.
	setValue(50);
}

// Handle event.
// Return 0 if ignored, else 1.
int Timer::subEventHandler(const df::Event* p_e) {

    // Parent handles event if score update.
    if (df::ViewObject::eventHandler(p_e))
        return 1;

    // Step events.
    if (p_e->getType() == df::STEP_EVENT)
        return step((df::EventStep*)p_e);

    // If get here, have ignored this event.
    return 0;
}

// Handle step events.
int Timer::step(const df::EventStep* p_e) {

    // Countdown the seconds.
    if (p_e->getStepCount() % 30 == 0 && getValue() > 0)
        setValue(getValue() - 1);
    else
        return 1;

#ifdef CLIENT
    // Sound warning as time expires.
    if (getValue() < 13 && getValue() % 2 == 0 ||
        getValue() < 6 && getValue() > 0)
        RM.getSound("beep")->play();

    // Time running out - yellow.
    if (getValue() <= 20 && getValue() > 10)
        setColor(df::YELLOW);

    // Time running out - red.
    if (getValue() < 10)
        setColor(df::RED);
#endif

    // Handled.
    return 1;
}

int Timer::serialize(std::stringstream* p_ss, unsigned int attr) {
    return df::ViewObject::serialize(p_ss, attr);
}

int Timer::deserialize(std::stringstream* p_ss, unsigned int* p_a){
    return df::ViewObject::deserialize(p_ss, p_a);
}

