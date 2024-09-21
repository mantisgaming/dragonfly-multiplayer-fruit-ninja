#pragma once

#include <Object.h>
#include <EventNetwork.h>
#include <EventStep.h>

class Server : df::Object {
private:
	int dataHandler(const df::EventNetwork* p_e);
	int acceptHandler(const df::EventNetwork* p_e);
	int closeHandler(const df::EventNetwork* p_e);
	int networkEventHandler(const df::EventNetwork* p_e);
	int stepHandler();
public:
	Server();
	int eventHandler(const df::Event* p_e) override;
};

