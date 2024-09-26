#include "Server.h"

#include <LogManager.h>
#include <NetworkManager.h>
#include <Sword.h>
#include <GameManager.h>
#include <Score.h>

#include "Grocer.h"

int Server::dataHandler(const df::EventNetwork* p_e) {
    LM.writeLog("INFO: Data received");
    const NetworkMessage& message = *(p_e->getMessage());
    switch (message.type) {

    case NetworkMessage::DISCONNECT:
        SetPlayerIDUsed(message.data[0], false);
        NM.close(p_e->getSocket());
        if (m_playersUsed == 0 && m_hasStarted)
            GM.setGameOver();
        return 1;

    case NetworkMessage::START_GAME:
        m_hasStarted = true;
        new Grocer();
        for (int i = 0; i < 4; i++) {
            if (isPlayerPresent(i)) {
                auto sc = new Score();
                sc->setPlayerID(i);
                sc->alocateObject();
                sc->synchronize();
            }
        }
        return 1;

    default:
        return 0;
    }
}

int Server::acceptHandler(const df::EventNetwork* p_e)
{
    LM.writeLog("INFO: Accepted connection");

    // only assign a sword if the game has not started yet
    if (!m_hasStarted) {

        auto ID = getFirstAvailablePlayerID();

        // assign the player ID
        NetworkMessage msg = { NetworkMessage::ASSIGN_CLIENT, reinterpret_cast<char*>(&ID), 1};
        p_e->getSocket()->send(msg);

        // spawn a sword for the player, if they are one of the first 4 and the game has not started yet
        if (ID < 4 && ID >= 0) {
            SetPlayerIDUsed(ID);

            auto sword = new Sword();
            sword->setPlayerID(ID);
            sword->alocateObject();
            sword->synchronize();
        }

        // give player 0 the start prompt if the game has not started yet
        if (ID == 0) {
            msg = { NetworkMessage::SHOW_START_PROMPT };
            p_e->getSocket()->send(msg);
        }

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

bool Server::isPlayerPresent(int8_t ID) {
    return (m_playersUsed >> ID) & 1;
}

Server::Server() {
    registerInterest(df::NETWORK_EVENT);
    m_playersUsed = 0;
    m_hasStarted = false;
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
