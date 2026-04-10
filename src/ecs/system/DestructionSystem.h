//
// Created by wifir on 2026-02-25.
//

#ifndef LECTURE8_DESTRUCTIONSYSTEM_H
#define LECTURE8_DESTRUCTIONSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

/*
 * DestructionSystem
 *
 * Responsible for removing entities (specifically projectiles) that leave the bounds of the game world.
 */
class DestructionSystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities)
    {
        // Find a camera entity
        Entity* cameraEntity = nullptr;
        for (auto& entity : entities)
        {
            if (entity->hasComponent<Camera>())
            {
                cameraEntity = entity.get();
                break;
            }
        }

        // // If no camera exists, cannot determine bounds
        if (!cameraEntity) return;

        // Check projectile bounds
        auto& camera = cameraEntity->getComponent<Camera>();

        for (auto& entity : entities)
        {
            // // Only process entities that are projectiles with positions
            if (entity->hasComponent<Transform>() && entity->hasComponent<ProjectileTag>())
            {
                // Check if outside world bounds
                auto& t = entity->getComponent<Transform>();
                if (t.position.x > camera.worldWidth ||
                    t.position.y > camera.worldHeight ||
                    t.position.x < 0 ||
                    t.position.y < 0)
                {
                    // Destroy entity.
                    entity->destroy();
                }
            }
        }
    }
};

#endif //LECTURE8_DESTRUCTIONSYSTEM_H