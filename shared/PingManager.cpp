#include "PingManager.h"

int PingManager::stepHandler(const df::EventStep* p_e) {
    #ifdef CLIENT
    if (m_timer <= 0) {
        m_timer = PING_INTERVAL;

        // TODO send ping message

        return 1;
    }
    #endif
    return 0;
}

int PingManager::networkHandler(const df::EventNetwork* p_e) {
    #ifdef CLIENT
        // TODO measure ping
    #else
        // TODO echo ping message
    #endif
    return 0;
}

PingManager::PingManager() {
    registerInterest(df::NETWORK_EVENT);

    #ifdef CLIENT
    m_timer = 0;
    registerInterest(df::STEP_EVENT);
    #endif
}

int PingManager::eventHandler(const df::Event* p_e) {
    #ifdef CLIENT
    if (p_e->getType() == df::STEP_EVENT)
        return stepHandler((df::EventStep*) p_e);
    #endif

    if (p_e->getType() == df::NETWORK_EVENT)
        return networkHandler((df::EventNetwork*)p_e);
    return 0;
}
