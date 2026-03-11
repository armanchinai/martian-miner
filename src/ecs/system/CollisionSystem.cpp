//
// Created by wifir on 2026-02-04.
//

#include "CollisionSystem.h"

#include "Collision.h"
#include "World.h"

void CollisionSystem::update(World& world)
{
    const std::vector<Entity*> collidables = queryCollidables(world.getEntities());

    for (size_t i = 0; i < collidables.size(); i++)
    {
        const auto entityA = collidables[i];
        const auto& t = entityA->getComponent<Transform>();
        auto& colA = entityA->getComponent<Collider>();

        colA.rect.x = t.position.x;
        colA.rect.y = t.position.y;

        for (size_t j = i + 1; j < collidables.size(); j++)
        {
            const auto entityB = collidables[j];

            if (auto& colB = entityB->getComponent<Collider>(); Collision::AABB(colA, colB))
            {
                world.getEventManager().emit(CollisionEvent{entityA, entityB});
                colA.isColliding = true;
                colB.isColliding = true;
            }

        }
    }
}

std::vector<Entity*> CollisionSystem::queryCollidables(const std::vector<std::unique_ptr<Entity>>& entities)
{
    std::vector<Entity*> collidables;
    for (auto& e : entities)
    {
        if (e->hasComponent<Collider>() && e->hasComponent<Transform>())
        {
            collidables.push_back(e.get());
        }
    }

    return collidables;
}
