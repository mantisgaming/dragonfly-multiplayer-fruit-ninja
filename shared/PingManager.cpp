#include "PingManager.h"

#include <GameManager.h>
#include <WorldManager.h>
#include <NetworkManager.h>
#include <EventView.h>

int PingManager::stepHandler(const df::EventStep* p_e) {
#ifdef CLIENT
    if (m_timer <= 0) {
        m_timer = PING_INTERVAL - 1;

        int time = GM.getStepCount();
        NetworkMessage msg = { NetworkMessage::PING, reinterpret_cast<char*>(&time), sizeof(int) };

        NM.sendToAll(msg);

        return 1;
    }

    m_timer--;
#endif
    
    return 0;
}

int PingManager::networkHandler(const df::EventNetwork* p_e) {
    if (p_e->getLabel() != df::EventNetwork::DATA)
        return 0;

    auto message = p_e->getMessage();

    if (message->type != NetworkMessage::PING)
        return 0;

#ifdef CLIENT
    int sendTime = *reinterpret_cast<const int*>(message->data);
    int time = GM.getStepCount();

    m_ping = time - sendTime;

    df::EventView ev = df::EventView("Ping: ", m_ping * GM.getFrameTime(), false);
    WM.onEvent(&ev);

#else
    p_e->getSocket()->send(*(p_e->getMessage()));
#endif
    
    return 1;
}

PingManager::PingManager() {
    registerInterest(df::NETWORK_EVENT);

#ifdef CLIENT
    m_timer = 0;
    m_ping = 0;
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
