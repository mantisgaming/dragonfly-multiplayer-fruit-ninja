#pragma once

#include "NetworkObject.h"
#include <EventOut.h>
#include <EventCollision.h>

const int NUM_FRUITS = 5;
const std::string FRUIT[NUM_FRUITS] = {
  "pear",
  "grapes",
  "apple",
  "banana",
  "blueberries"
};

const std::string FRUIT_STRING = "Fruit";
const uint8_t FRUIT_TYPE_ID = 1;

const int EXPLOSION_AGE = 45;        // in ticks
const float EXPLOSION_SPEED = 0.05f; // in spaces/tick
const float EXPLOSION_ROTATE = 1.0f; // in degrees

class Fruit : public NetworkObject {
private:
	void explode();
	std::string m_sprite;
	bool m_playSound;
public:
	Fruit(const std::string& sprite);
	Fruit(uint8_t netID = UINT8_MAX);
	~Fruit();
	int subEventHandler(const df::Event* p_e) override;
	int out(const df::EventOut* p_e);
	int collide(const df::EventCollision* p_e);
	bool isExiting() const;
	void start(float speed);
	int serialize(std::stringstream* p_ss, unsigned int attr = 0) override;
	int deserialize(std::stringstream* p_ss, unsigned int* p_a = NULL) override;
};

