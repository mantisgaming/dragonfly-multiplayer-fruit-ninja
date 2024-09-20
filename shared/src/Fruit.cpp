#include "Fruit.h"

#include <LogManager.h>
#include <WorldManager.h>
#include <GameManager.h>
#include <ResourceManager.h>
#include <EventView.h>

#include "NetworkManager.h"
#include "Sword.h"

void Fruit::explode() {
    #ifdef CLIENT
        df::explode(getAnimation().getSprite(), getAnimation().getIndex(), getPosition(),
            EXPLOSION_AGE, EXPLOSION_SPEED, EXPLOSION_ROTATE);

        std::string sound = "splat-" + std::to_string(rand() % 6 + 1);
        RM.getSound(sound)->play();
        WM.markForDelete(this);
    #endif
}

Fruit::Fruit(std::string& sprite) : NetworkObject(NetworkObject::getUniqueID(), 30){
	setType(FRUIT_STRING);

	if (setSprite(sprite) && NM.isClient()) {
		LM.writeLog("Failed to find sprite '%s'", sprite.c_str());
	}

	setSolidness(df::SOFT);
}

Fruit::Fruit() {
    setType(FRUIT_STRING);

}

int Fruit::subEventHandler(const df::Event* p_e) {
    if (p_e->getType() == df::OUT_EVENT)
        return out((df::EventOut*)p_e);

    if (p_e->getType() == df::COLLISION_EVENT)
        return collide((df::EventCollision*)p_e);

    return 0;
}

int Fruit::out(const df::EventOut* p_e) {
    #ifdef SERVER
        if (!isExiting()) { return 1; }

        // TODO trigger fruit miss event

        WM.markForDelete(this);
        return 1;
    #endif

    return 0;
}

int Fruit::collide(const df::EventCollision* p_e) {
    #ifdef SERVER
        if (p_e->getObject1()->getType() == SWORD_STRING) {

            // TODO trigger fruit hit event

            WM.markForDelete(this);

            explode();
        }
        return 1;
    #endif

    return 0;
}

// if the dot product between the direction of movement
// and the position relative to the center of the screen
// is negative, then it is exiting
bool Fruit::isExiting() const {
    int world_x = (int)WM.getBoundary().getHorizontal();
    int world_y = (int)WM.getBoundary().getVertical();
    df::Vector world_center(world_x / 2.0f, world_y / 2.0f);

    df::Vector difference = world_center - getPosition();
    difference.normalize();

    float dot = world_center.getX() * difference.getX() + world_center.getY() * difference.getY();

    return dot < 0;
}

void Fruit::start(float speed) {
    df::Vector begin, end;

    // Get world boundaries.
    int world_x = (int)WM.getBoundary().getHorizontal();
    int world_y = (int)WM.getBoundary().getVertical();
    df::Vector world_center(world_x / 2.0f, world_y / 2.0f);

    // Pick random side (Top, Right, Bottom, Left) to spawn.
    switch (rand() % 4) {
    case 0: // Top.
        begin = df::Vector((float)(rand() % world_x),       0 - 3.0f);
        end =   df::Vector((float)(rand() % world_x), world_y + 3.0f);
        break;

    case 1: // Right.
        begin = df::Vector(world_x + 3.0f, (float)(rand() % world_y));
        end =   df::Vector(      0 - 3.0f, (float)(rand() % world_y));
        break;

    case 2: // Bottom.
        begin = df::Vector((float)(rand() % world_x), world_y + 3.0f);
        end =   df::Vector((float)(rand() % world_x),       0 - 3.0f);
        break;

    case 3: // Left.
        begin = df::Vector(      0 - 3.0f, (float)(rand() % world_y));
        end =   df::Vector(world_x + 3.0f, (float)(rand() % world_y));
        break;
    }

    // Set velocity towards opposite side.
    df::Vector velocity = end - begin;
    velocity.normalize();
    setDirection(velocity);
    setSpeed(speed);

    // Move Object into position.
    WM.moveObject(this, begin);

    synchronize();
}