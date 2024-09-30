#include "NetworkSentry.h"

#include "NetworkManager.h"
#include "NetworkObject.h"
#include "WorldManager.h"
#include "Fruit.h"
#include "Sword.h"
#include "Timer.h"
#include "Score.h"

namespace df {

    int NetworkSentry::stepHandler(const EventStep* p_event)
    {
        NM.accept();
        NM.flush();
        NM.recieve();
        return 1;
    }

    int NetworkSentry::networkHandler(const EventNetwork* p_e)
    {
        if (p_e->getLabel() == df::EventNetwork::DATA) {

            const NetworkMessage& message = *p_e->getMessage();

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
                    switch (typeID) {
                    case FRUIT_TYPE_ID:
                        object = new Fruit();
                        break;
                    case SWORD_TYPE_ID:
                        object = new Sword();
                        break;
                    case TIMER_TYPE_ID:
                        object = new Timer();
                        break;
                    case SCORE_TYPE_ID:
                        object = new Score();
                        break;
                    default:
                        return 0;
                    }
                    object->setNetID(netID);
                    NetworkObject::registerObject(object);
                }
                object->deserialize(&stream);
                return 1;

            case NetworkMessage::DESTROY:
                if (object != NULL) 
                    WM.markForDelete(object);
                return 1;

#ifdef CLIENT
            case NetworkMessage::ASSIGN_CLIENT:
                NM.setClientID((int8_t)message.data[0]);
                return 1;
#endif
            default:
                return 0;
            }
        }
        else if (p_e->getLabel() == df::EventNetwork::ACCEPT) {
            
            for (int i = 0; i < UINT8_MAX; i++) {
                auto obj = NetworkObject::getObject(i);
                if (obj == NULL)
                    continue;

                obj->synchronize(-1);
            }

            return 1;
        }

        return 0;
    }

    NetworkSentry::NetworkSentry()
    {
        registerInterest(STEP_EVENT);
        registerInterest(NETWORK_EVENT);
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