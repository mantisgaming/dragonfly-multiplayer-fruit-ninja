#include "Client.h"

#include <LogManager.h>
#include <NetworkManager.h>
#include <NetworkObject.h>
#include <Fruit.h>

#include "GameOver.h"
#include "StartButton.h"

Client* Client::instance = NULL;

int Client::dataHandler(const df::EventNetwork* p_e) {
    LM.writeLog("INFO: Data received");
    const NetworkMessage& message = *(p_e->getMessage());

    std::stringstream stream;
    stream.write(message.data, message.dataSize);

    switch (message.type) {

    case NetworkMessage::DISCONNECT:
        p_e->getSocket()->close();
        return 1;

    case NetworkMessage::GAME_OVER:
        new GameOver();
        return 1;

    case NetworkMessage::SHOW_START_PROMPT:
        new StartButton();
        return 1;
    default:
        return 0;
    }
}

int Client::connectHandler(const df::EventNetwork* p_e)
{
    LM.writeLog("INFO: Connected to server");
    return 0;
}

int Client::closeHandler(const df::EventNetwork* p_e)
{
    LM.writeLog("INFO: Connection closed");
    return 0;
}

int Client::networkEventHandler(const df::EventNetwork* p_e)
{
    switch (p_e->getLabel()) {

    case df::EventNetwork::CONNECT:
        return connectHandler(p_e);

    case df::EventNetwork::CLOSE:
        return closeHandler(p_e);

    case df::EventNetwork::DATA:
        return dataHandler(p_e);

    default:
        return 0;
    }
}

int Client::stepHandler() {
    NM.checkConnections();
    return 1;
}

Client::Client() {
    instance = this;
    m_playerID = -1;
    registerInterest(df::NETWORK_EVENT);
}

int Client::eventHandler(const df::Event* p_e)
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

Client& Client::getInstance() {
    return *instance;
}
