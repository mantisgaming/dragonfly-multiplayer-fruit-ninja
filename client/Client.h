#pragma once

#include <Object.h>
#include <EventNetwork.h>
#include <EventStep.h>

class Client : df::Object {
private:
	void spawnObject(std::stringstream& stream);
	int dataHandler(const df::EventNetwork* p_e);
	int connectHandler(const df::EventNetwork* p_e);
	int closeHandler(const df::EventNetwork* p_e);
	int networkEventHandler(const df::EventNetwork* p_e);
	int stepHandler();
public:
	Client();
	int eventHandler(const df::Event* p_e) override;
};

