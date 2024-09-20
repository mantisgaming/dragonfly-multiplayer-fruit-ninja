#pragma once

#include <Object.h>
#include <EventNetwork.h>

class Server : df::Object {
private:
	int networkEventHandler(const df::EventNetwork* p_e);
public:
	Server();
	int eventHandler(const df::Event* p_e) override;
};

