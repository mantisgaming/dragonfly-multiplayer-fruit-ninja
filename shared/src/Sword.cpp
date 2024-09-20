#include "Sword.h"
#include "DisplayManager.h"
#include "NetworkManager.h"

int Sword::stepEvent() {
	return 0;
}

Sword::Sword(df::Color color) {
	m_color = color;
	m_ultimateStep = 0;
	m_mana = 0;
	m_old_sliced = 0;
	m_sliced = 0;
	m_old_position = df::Vector(0,0);
}

int Sword::subEventHandler(const df::Event* p_e) {
	return 0;
}

int Sword::serialize(std::stringstream* p_ss, unsigned int attr) {
	int ok = Object::serialize(p_ss, attr);

	// TODO serialize sword

	return ok;
}

int Sword::deserialize(std::stringstream* p_ss, unsigned int* p_a) {
	int ok = Object::deserialize(p_ss, p_a);

	// TODO deserialize sword
	
	return ok;
}

int Sword::draw()
{
	return DM.drawCh(getPosition(), SWORD_CHAR, m_color);
}
