#include "EventScore.h"

EventScore::EventScore(int8_t playerID, int8_t scoreChange) {
    m_playerID = playerID;
    m_scoreChange = scoreChange;
    setType(SCORE_EVENT);
}
