#pragma once

#include <Object.h>
#include <EventNetwork.h>
#include <EventStep.h>

class Client : df::Object {
private:
	int dataHandler(const df::EventNetwork* p_e);
	int connectHandler(const df::EventNetwork* p_e);
	int closeHandler(const df::EventNetwork* p_e);
	int networkEventHandler(const df::EventNetwork* p_e);
	int stepHandler();
	int8_t m_playerID;
	static Client* instance;
public:
	Client();
	int eventHandler(const df::Event* p_e) override;
	static Client& getInstance();
};

