#include "Score.h"
#include "Sword.h"
#include "NetworkManager.h"

int8_t Score::positionIndex = 0;
const df::ViewObjectLocation Score::positions[3] = {df::BOTTOM_RIGHT, df::BOTTOM_LEFT, df::BOTTOM_CENTER};

int Score::scoreHandler(const EventScore* p_e) {
    if (p_e->getPlayerID() == getPlayerID() || p_e->getPlayerID() == -1) {
        
        int newVal = getValue() + p_e->getScoreChange();
        newVal = newVal < 0? 0 : newVal;

        setValue(newVal);
        synchronize();

        return 1;
    }
    return 0;
}

Score::Score() : NetworkObject(SCORE_TYPE_ID) {
    m_playerID = -1;
    setBorder(true);
    setLocation(df::CENTER_CENTER);
    setViewString("Score: ");
    setValue(0);
    df::Object::registerInterest(SCORE_EVENT);
}

void Score::setPlayerID(int8_t playerID) {
    m_playerID = playerID;
    setColor(Sword::getColor(playerID));
}

int8_t Score::getPlayerID() {
    return m_playerID;
}

int Score::serialize(std::stringstream* p_ss, const unsigned int attr) {
    int ok = df::ViewObject::serialize(p_ss, attr);
    p_ss->put(m_playerID);

    ok |= !p_ss->good();
    return ok;
}

int Score::deserialize(std::stringstream* p_ss, unsigned int* p_a) {
    int ok = df::ViewObject::deserialize(p_ss, p_a);

    int8_t oldID = m_playerID;

    p_ss->get((char&)m_playerID);

    if (m_playerID != oldID) {
        if (m_playerID == NM.getClientID()) {
            setLocation(df::TOP_RIGHT);
        } else {
            setLocation(positions[positionIndex % 3]);
            positionIndex++;
        }
    }

    ok |= !p_ss->good();
    return ok;
}

int Score::eventHandler(const df::Event* p_e) {
    if (p_e->getType() == SCORE_EVENT)
        return scoreHandler((EventScore*) p_e);
    return 0;
}
