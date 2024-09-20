#include "Sword.h"
#include "DisplayManager.h"
#include "NetworkManager.h"

int Sword::stepEvent() {
	CLIENT_ONLY (return clientStep();)
	SERVER_ONLY (return serverStep();)
	return 0;
}

int Sword::clientStep() {

	// TODO draw particles and send mouse data

	return 1;
}

int Sword::serverStep() {

	// TODO process sword movement

	return 1;
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
