//
// Created by wifir on 2026-02-04.
//

#ifndef LECTURE5_COLLISIONSYSTEM_H
#define LECTURE5_COLLISIONSYSTEM_H
#include <memory>
#include <vector>

#include "Entity.h"

class World;

class CollisionSystem
{
public:
    void update(World& world);
private:
    std::vector<Entity*> queryCollidables(const std::vector<std::unique_ptr<Entity>>& entities);
};

#endif //LECTURE5_COLLISIONSYSTEM_H