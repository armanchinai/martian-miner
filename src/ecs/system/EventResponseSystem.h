//
// Created by wifir on 2026-03-30.
//

#ifndef MARTIAN_MINER_EVENTRESPONSESYSTEM_H
#define MARTIAN_MINER_EVENTRESPONSESYSTEM_H
#include <functional>

#include "events/BaseEvent.h"

class Entity;
class World;

class EventResponseSystem {
public:
    EventResponseSystem(World& world);
private:
    void onCollision(const CollisionEvent& e, const char* otherTag, World& world);
    bool getCollisionEntities(const CollisionEvent& e, const char* otherTag, Entity*& player, Entity*& other);

    void onPlayerAction(const PlayerAction& e, const std::function<void(Entity* player, PlayerAction action)>& callback);
};

#endif //MARTIAN_MINER_EVENTRESPONSESYSTEM_H
