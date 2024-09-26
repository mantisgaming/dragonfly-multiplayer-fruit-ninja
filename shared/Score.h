#pragma once

#include <ViewObject.h>
#include <stdint.h>
#include "NetworkObject.h"
#include "EventScore.h"

const uint8_t SCORE_TYPE_ID = 4;

class Score : public NetworkObject, public df::ViewObject {
private:
	int8_t m_playerID;
	static int8_t positionIndex;
	static const df::ViewObjectLocation positions[3];
	int scoreHandler(const EventScore* p_e);
public:
	Score();
	void setPlayerID(int8_t playerID);
	int8_t getPlayerID();
	int serialize(std::stringstream* p_ss, const unsigned int attr = 0) override;
	int deserialize(std::stringstream* p_ss, unsigned int* p_a = NULL) override;
	int eventHandler(const df::Event* p_e) override;
};

