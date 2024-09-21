#pragma once

#include "NetworkObject.h"
#include "EventNetwork.h"

const std::string SWORD_STRING = "Sword";
const uint8_t SWORD_TYPE_ID = 2;

const char SWORD_CHAR = '+';

class Sword : NetworkObject {
private:
	df::Color m_color;
	df::Vector m_old_position;
	int m_sliced;
	int m_old_sliced;
	int m_ultimateStep;
	int m_mana;

	int stepEvent();
public:
	Sword(df::Color color = df::Color::RED);
	int subEventHandler(const df::Event* p_e) override;
	int serialize(std::stringstream* p_ss, unsigned int attr = 0) override;
	int deserialize(std::stringstream* p_ss, unsigned int* p_a = NULL) override;
	int draw() override;
};
