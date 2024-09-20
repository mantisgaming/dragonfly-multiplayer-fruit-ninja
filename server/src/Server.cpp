#include "Server.h"

int Server::networkEventHandler(const df::EventNetwork* p_e)
{
    switch (p_e->getLabel()) {
    case df::EventNetwork::ACCEPT:
        return 1;
    case df::EventNetwork::CLOSE:
        return 1;
    case df::EventNetwork::DATA:
        return 1;
    default:
        return 0;
    }
}

Server::Server() {
    registerInterest(df::NETWORK_EVENT);
}

int Server::eventHandler(const df::Event* p_e)
{
    if (p_e->getType() == df::NETWORK_EVENT) {
        return networkEventHandler((df::EventNetwork*)p_e);
    }
    return 0;
}
