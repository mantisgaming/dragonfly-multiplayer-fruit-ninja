//
// Grocer.h
//

// Engine includes.
#include "EventStep.h"
#include "Object.h"

// Game includes.
#include <Fruit.h>

// Wave settings.
const int NUM_WAVES = NUM_FRUITS;
const int WAVE_LEN = 300;      // in ticks
const int WAVE_SPAWN = 30;     // in ticks
const float WAVE_SPEED = 0.5f; // in ticks
const float SPEED_INC = 0.1f;  // in spaces/tick
const int SPAWN_INC = -5;     // in ticks

const std::string GROCER_STRING = "Grocer";

class Grocer : public df::Object {

private:
	int m_spawn;        // countdown to next fruit, in ticks
	int m_wave;         // wave number
	int m_wave_spawn;   // current wave countdown, in ticks
	int m_wave_end;     // current wave end, in ticks
	float m_wave_speed; // current fruit speed, in spaces/tick

	// Handle step events.
	int step(const df::EventStep* p_e);

public:

	// Constructor.
	Grocer();

	// Handle events.
	int eventHandler(const df::Event* p_e) override;

	// Do game over actions.
	void gameOver();

	// override serialization
	int serialize(std::stringstream* p_ss, unsigned int mask = 0) override;
	int deserialize(std::stringstream* p_ss, unsigned int* p_a = NULL) override;
};
