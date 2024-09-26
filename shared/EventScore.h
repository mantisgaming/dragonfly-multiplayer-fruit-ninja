#pragma once

#include <Event.h>

const std::string SCORE_EVENT = "score-event";

class EventScore : public df::Event {
private:
	int8_t m_playerID;
	int8_t m_scoreChange;
public:
	EventScore(int8_t playerID, int8_t scoreChange);
	constexpr int8_t getPlayerID() const { return m_playerID; };
	constexpr int8_t getScoreChange() const { return m_scoreChange; };
};

