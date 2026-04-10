//
// Created by wifir on 2026-02-04.
//

#include "CollisionSystem.h"
#include "Collision.h"
#include "World.h"
#include <set>

/*
 * CollisionSystem
 *
 * Responsible for detecting collisions between entities and emitting collision events (Enter, Stay, Exit). Relies on
 * methods from World and EventManager, as well as the Collision::AABB function.
 */
void CollisionSystem::update(World& world)
{
    // Get all entities that can participate in collisions
    const std::vector<Entity*> collidables = queryCollidables(world.getEntities());
    // Stores collisions detected in this frame
    std::set<CollisionKey> currentCollisions{};

    // Check all entity pairs
    for (size_t i = 0; i < collidables.size(); i++)
    {
        // Get Transform and Collider for entity A
        const auto entityA = collidables[i];
        const auto& t = entityA->getComponent<Transform>();
        auto& colA = entityA->getComponent<Collider>();

        // Sync collider position with transform
        colA.rect.x = t.position.x;
        colA.rect.y = t.position.y;

        // Compare with all remaining entities (avoid duplicate pairs)
        for (size_t j = i + 1; j < collidables.size(); j++)
        {
            const auto entityB = collidables[j];

            // Perform AABB collision check
            if (auto& colB = entityB->getComponent<Collider>(); Collision::AABB(colA, colB))
            {
                // Create a unique key for this entity pair
                CollisionKey key = makeKey(entityA, entityB);
                // Record collision for this frame
                currentCollisions.insert(key);

                // Emit Collision event.
                if (!activeCollisions.contains(key))
                {
                    // Collision started this frame
                    world.getEventManager().emit(CollisionEvent{entityA, entityB, CollisionState::Enter});
                }
                else
                {
                    // Collision is ongoing
                    world.getEventManager().emit(CollisionEvent{entityA, entityB, CollisionState::Stay});
                }
            }
        }
    }

    // Detect Exit events
    for (auto& key : activeCollisions)
    {
        if (!currentCollisions.contains(key))
        {
            // Collision ended this frame
            world.getEventManager().emit(CollisionEvent{key.first, key.second, CollisionState::Exit});
        }
    }

    // Update active collisions
    activeCollisions = std::move(currentCollisions);
}

/*
 * queryCollidables
 *
 * Filters entities that can participate in collision detection.
 *
 * Returns:
 * - A list of Entity pointers that have both Collider and Transform
 */
std::vector<Entity*> CollisionSystem::queryCollidables(const std::vector<std::unique_ptr<Entity>>& entities)
{
    std::vector<Entity*> collidables;
    for (auto& e : entities)
    {
        // // Only include entities with required components
        if (e->hasComponent<Collider>() && e->hasComponent<Transform>())
        {
            collidables.push_back(e.get());
        }
    }

    return collidables;
}
