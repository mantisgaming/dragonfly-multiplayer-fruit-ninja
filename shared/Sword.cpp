#include "Sword.h"

#include <utility.h>
#include <DisplayManager.h>
#include <EventCollision.h>
#include <WorldManager.h>
#include <GameManager.h>

#include "NetworkManager.h"
#include "Util.h"
#include "Fruit.h"

int Sword::mouseHandler(df::EventMouse* p_e) {
	if (!belongsToClient()) {
		unregisterInterest(df::MSE_EVENT);
		return 0;
	}

	df::Vector mousePos = p_e->getMousePosition();

	char data[9];
	data[0] = m_playerID;
	*reinterpret_cast<float*>(&data[1]) = mousePos.getX();
	*reinterpret_cast<float*>(&data[5]) = mousePos.getY();

	NetworkMessage msg = { NetworkMessage::SWORD_POSITION, data, 9 };
	NM.sendToAll(msg);

	return 1;
}

int Sword::networkHandler(df::EventNetwork* p_e) {
	if (p_e->getLabel() != df::EventNetwork::DATA) return 0;

	const NetworkMessage* message = p_e->getMessage();

#ifdef SERVER
	switch (message->type) {
	case NetworkMessage::SWORD_POSITION:
	{
		if (m_playerID != (int8_t)message->data[0]) return 0;

		df::Vector pos;
		m_old_position = getPosition();
		pos.setXY(*reinterpret_cast<const float*>(&message->data[1]), *reinterpret_cast<const float*>(&message->data[5]));

		setPosition(pos);
		synchronize();

		handleCollisions();

		return 1;
	}
	case NetworkMessage::DISCONNECT:
		if (m_playerID != (int8_t)message->data[0]) return 0;
		WM.markForDelete(this);
		return 1;
	default:
		return 0;
	}
#else
	switch (message->type) {
	case NetworkMessage::SYNC:
		if (getNetworkID() != (int8_t)message->data[0]) return 0;
		
		Util::drawTrail(getPosition(), m_old_position, getColor());

		// TODO spawn kudos
		//// Spawn kudos for combo.
		//if (m_sliced > 2 && m_sliced > m_old_sliced)
		//	new Kudos();

		// If travel far enough, play "swipe" sound.
		float dist = df::distance(getPosition(), m_old_position);
		if (dist > 15) {
			std::string sound = "swipe-" + std::to_string(rand() % 7 + 1);
			play_sound(sound);
		}

		return 1;

	default:
		return 0;
	}
#endif
}

void Sword::handleCollisions() {

	// Check if line intersects any Fruit objects.
	df::Line line(getPosition(), m_old_position);
	df::ObjectList ol = WM.solidObjects();

	for (int i = 0; i < ol.getCount(); i++) {

		// Only slice Fruit.
		if (!(dynamic_cast <Fruit*> (ol[i])))
			continue;

		// If line from previous position intersects --> slice!
		df::Object* p_o = ol[i];
		df::Box box = getWorldBox(p_o);
		if (lineIntersectsBox(line, box)) {
			df::EventCollision c(this, p_o, p_o->getPosition());
			p_o->eventHandler(&c);
			m_sliced += 1;

			m_old_sliced = m_sliced;

		}

		// TODO lose points
		// Lose points for distance traveled
		//int penalty = -1 * (int)(dist / 10.0f);
		//df::EventView ev(POINTS_STRING, penalty, true);
		//WM.onEvent(&ev);

	}

}

Sword::Sword() : NetworkObject(SWORD_TYPE_ID) {
	m_ultimateStep = 0;
	m_mana = 0;
	m_old_sliced = 0;
	m_sliced = 0;
	m_playerID = -1;

	setType(SWORD_STRING);
	setSolidness(df::SPECTRAL);
	setAltitude(df::MAX_ALTITUDE); // Make Sword in foreground.

	// Start sword in center of world.
	df::Vector p(WM.getBoundary().getHorizontal() / 2,
		WM.getBoundary().getVertical() / 2);
	setPosition(p);

	m_old_position = p;

	registerInterest(df::MSE_EVENT);
	registerInterest(df::NETWORK_EVENT);
}

int Sword::subEventHandler(const df::Event* p_e) {
	if (p_e->getType() == df::MSE_EVENT)
		return mouseHandler((df::EventMouse*)p_e);

	if (p_e->getType() == df::NETWORK_EVENT)
		return networkHandler((df::EventNetwork*)p_e);

	return 0;
}

int Sword::serialize(std::stringstream* p_ss, unsigned int attr) {
	int ok = Object::serialize(p_ss, attr);

	m_old_position.serialize(p_ss);
	p_ss->write(reinterpret_cast<char*>(&m_sliced),sizeof(m_sliced));
	p_ss->write(reinterpret_cast<char*>(&m_old_sliced), sizeof(m_old_sliced));
	p_ss->write(reinterpret_cast<char*>(&m_mana), sizeof(m_mana));
	p_ss->write(reinterpret_cast<char*>(&m_playerID), sizeof(m_playerID));

	ok |= !p_ss->good();
	return ok;
}

int Sword::deserialize(std::stringstream* p_ss, unsigned int* p_a) {
	int ok = Object::deserialize(p_ss, p_a);

	m_old_position.deserialize(p_ss);
	p_ss->read(reinterpret_cast<char*>(&m_sliced), sizeof(m_sliced));
	p_ss->read(reinterpret_cast<char*>(&m_old_sliced), sizeof(m_old_sliced));
	p_ss->read(reinterpret_cast<char*>(&m_mana), sizeof(m_mana));
	p_ss->read(reinterpret_cast<char*>(&m_playerID), sizeof(m_playerID));

	ok |= !p_ss->good();
	return ok;
}

int Sword::draw()
{
	return DM.drawCh(getPosition(), SWORD_CHAR, getColor());
}

int8_t Sword::getPlayerID() const {
	return m_playerID;
}

void Sword::setPlayerID(int8_t ID) {
	m_playerID = ID;
}

df::Color Sword::getColor() {
	if (m_playerID < 0)
		return df::COLOR_DEFAULT;
	return SWORD_COLORS[m_playerID % SWORD_COLOR_COUNT];
}

bool Sword::belongsToClient() {
	return m_playerID == NM.getClientID();
}
