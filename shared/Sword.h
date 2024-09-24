#pragma once

#include <EventStep.h>
#include <EventMouse.h>

#include "NetworkObject.h"

const std::string SWORD_STRING = "Sword";
const uint8_t SWORD_TYPE_ID = 2;

const char SWORD_CHAR = '+';

const int SWORD_COLOR_COUNT = 4;
const df::Color SWORD_COLORS[SWORD_COLOR_COUNT]
	= { df::Color::RED, df::Color::CYAN, df::Color::YELLOW, df::Color::GREEN };

class Sword : public NetworkObject {
private:
	df::Vector m_old_position;
	int8_t m_playerID;
	int m_sliced;
	int m_old_sliced;
	int m_mana;
	int m_ultimateStep;

	int mouseHandler(df::EventMouse* p_e);
	int networkHandler(df::EventNetwork* p_e);
	void handleCollisions();
public:
	Sword();
	int subEventHandler(const df::Event* p_e) override;
	int serialize(std::stringstream* p_ss, unsigned int attr = 0) override;
	int deserialize(std::stringstream* p_ss, unsigned int* p_a = NULL) override;
	int draw() override;
	int8_t getPlayerID() const;
	void setPlayerID(int8_t ID);
	df::Color getColor();
	bool belongsToClient();
};
