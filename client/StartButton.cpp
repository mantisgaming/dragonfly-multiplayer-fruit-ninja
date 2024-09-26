#include "StartButton.h"

#include <WorldManager.h>

#include <NetworkManager.h>

StartButton::StartButton() {
	setType("StartButton");
	setLocation(df::ViewObjectLocation::BOTTOM_CENTER);
	setDefaultColor(df::Color::WHITE);
	setHighlightColor(df::Color::GREEN);
	setBorder(true);
	setViewString("Start Game");
}

void StartButton::callback() {
	NetworkMessage msg = { NetworkMessage::START_GAME };
	NM.sendToAll(msg);
	WM.markForDelete(this);
}
