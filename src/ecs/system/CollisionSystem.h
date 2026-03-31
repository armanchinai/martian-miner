//
// Created by wifir on 2026-02-04.
//

#ifndef LECTURE5_COLLISIONSYSTEM_H
#define LECTURE5_COLLISIONSYSTEM_H
#include <algorithm>
#include <memory>
#include <vector>
#include <set>

#include "Entity.h"

class World;

using CollisionKey = std::pair<Entity*, Entity*>;

class CollisionSystem
{
public:
    void update(World& world);
    std::set<CollisionKey> activeCollisions;
private:
    std::vector<Entity*> queryCollidables(const std::vector<std::unique_ptr<Entity>>& entities);
    static CollisionKey makeKey(Entity* entityA, Entity* entityB)
    {
        return std::minmax(entityA, entityB);
    }
};

#endif //LECTURE5_COLLISIONSYSTEM_H