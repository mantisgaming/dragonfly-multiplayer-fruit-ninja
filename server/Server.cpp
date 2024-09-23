#include "Server.h"

#include <LogManager.h>
#include <NetworkManager.h>

#include "Sword.h"

int Server::dataHandler(const df::EventNetwork* p_e) {
    LM.writeLog("INFO: Data received");
    const NetworkMessage& message = *(p_e->getMessage());
    switch (message.type) {

    case NetworkMessage::DISCONNECT:
        SetPlayerIDUsed(message.data[0], false);
        NM.close(p_e->getSocket());
        return 1;

    default:
        return 0;
    }
}

int Server::acceptHandler(const df::EventNetwork* p_e)
{
    LM.writeLog("INFO: Accepted connection");
    auto ID = getFirstAvailablePlayerID();

    char data = ID;

    NetworkMessage msg = { NetworkMessage::ASSIGN_CLIENT, &data, 1 };
    p_e->getSocket()->send(msg);

    if (ID < 4 && ID >= 0) {
        SetPlayerIDUsed(ID);

        auto sword = new Sword();
        sword->setPlayerID(ID);
        sword->alocateObject();
        sword->synchronize();
    }

    return 1;
}

int Server::closeHandler(const df::EventNetwork* p_e)
{
    LM.writeLog("INFO: Connection closed");
    return 0;
}

int Server::networkEventHandler(const df::EventNetwork* p_e)
{
    switch (p_e->getLabel()) {

    case df::EventNetwork::ACCEPT:
        return acceptHandler(p_e);

    case df::EventNetwork::CLOSE:
        return closeHandler(p_e);

    case df::EventNetwork::DATA:
        return dataHandler(p_e);

    default:
        return 0;
    }
}

int Server::stepHandler() {
    NM.checkConnections();
    return 1;
}

int8_t Server::getFirstAvailablePlayerID() {
    uint8_t playerTmp = m_playersUsed;
    for (int i = 0; i < 4; i++) {
        if (!(playerTmp & 1))
            return i;
        playerTmp >>= 1;
    }
    return -1;
}

void Server::SetPlayerIDUsed(int8_t ID, bool used) {
    if (ID < 0 || ID >= 4) {
        LM.writeLog("WARNING: Server:SetPlayerIDUsed(): Attempted to assign player ID %c", ID);
        return;
    }

    m_playersUsed |= used ? (1 << ID) : 0;
    m_playersUsed &= 0xFF ^ ((!used) ? (1 << ID) : 0);
}

Server::Server() {
    registerInterest(df::NETWORK_EVENT);
    m_playersUsed = 0;
}

int Server::eventHandler(const df::Event* p_e)
{
    auto eventType = p_e->getType();
    if (eventType == df::NETWORK_EVENT) {
        return networkEventHandler((df::EventNetwork*)p_e);
    }
    else if (eventType == df::STEP_EVENT) {
        return stepHandler();
    }
    return 0;
}
