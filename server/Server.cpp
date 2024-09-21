#include "Server.h"

#include <LogManager.h>
#include <NetworkManager.h>

int Server::dataHandler(const df::EventNetwork* p_e) {
    const NetworkMessage& message = *(p_e->getMessage());
    switch (message.type) {

    case NetworkMessage::DISCONNECT:
        p_e->getSocket()->close();
        return 1;

    default:
        return 0;
    }
}

int Server::acceptHandler(const df::EventNetwork* p_e)
{
    return 0;
}

int Server::closeHandler(const df::EventNetwork* p_e)
{
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

Server::Server() {
    registerInterest(df::NETWORK_EVENT);
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
