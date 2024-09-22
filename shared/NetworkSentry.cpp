#include "NetworkSentry.h"

#include "NetworkManager.h"
#include "NetworkObject.h"
#include "WorldManager.h"

namespace df {

    int NetworkSentry::stepHandler(const EventStep* p_event)
    {
        NM.accept();
        NM.recieve();
        return 1;
    }

    int NetworkSentry::networkHandler(const EventNetwork* p_e)
    {
        if (p_e->getLabel() != df::EventNetwork::DATA) return 0;

        const NetworkMessage& message = *p_e->getMessage();
        if (message.type != NetworkMessage::SYNC && message.type != NetworkMessage::DESTROY) return 0;
        
        uint8_t netID;
        uint8_t typeID;

        std::stringstream stream;
        stream.write(message.data, message.dataSize);
        stream.read(reinterpret_cast<char*>(&netID), 1);
        stream.read(reinterpret_cast<char*>(&typeID), 1);

        auto object = NetworkObject::getObject(netID);

        switch (message.type) {
        case NetworkMessage::SYNC:
            if (object == NULL) {
                // TODO create object
            }
            deserialize(&stream);
            break;
        case NetworkMessage::DESTROY:
            if (object != NULL)
                WM.markForDelete(this);
            break;
        }

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
        else if (p_event->getType() == NETWORK_EVENT) {
            return networkHandler((EventNetwork*)p_event);
        }
        return 0;
    }

}