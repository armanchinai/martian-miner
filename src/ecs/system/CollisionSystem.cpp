//
// Created by wifir on 2026-02-04.
//

#include "CollisionSystem.h"
#include "Collision.h"
#include "World.h"
#include <set>

void CollisionSystem::update(World& world)
{
    const std::vector<Entity*> collidables = queryCollidables(world.getEntities());
    std::set<CollisionKey> currentCollisions{};

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
                CollisionKey key = makeKey(entityA, entityB);
                currentCollisions.insert(key);

                if (!activeCollisions.contains(key))
                {
                    world.getEventManager().emit(CollisionEvent{entityA, entityB, CollisionState::Enter});
                }
                else
                {
                    world.getEventManager().emit(CollisionEvent{entityA, entityB, CollisionState::Stay});
                }
            }
        }
    }

    for (auto& key : activeCollisions)
    {
        if (!currentCollisions.contains(key))
        {
            world.getEventManager().emit(CollisionEvent{key.first, key.second, CollisionState::Exit});
        }
    }

    activeCollisions = std::move(currentCollisions);
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
