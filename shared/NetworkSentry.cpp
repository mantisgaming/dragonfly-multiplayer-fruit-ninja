#include "NetworkSentry.h"

#include "NetworkManager.h"

namespace df {

    int NetworkSentry::stepHandler(const EventStep* p_event)
    {
        if (NM.isServer()) {
            NM.accept();
        }

        NM.recieve();

        return 1;
    }

    NetworkSentry::NetworkSentry()
    {
        registerInterest(STEP_EVENT);
    }

    int NetworkSentry::eventHandler(const Event* p_event)
    {
        if (p_event->getType() == STEP_EVENT) {
            return stepHandler((EventStep*)p_event);
        }
        return 0;
    }

}