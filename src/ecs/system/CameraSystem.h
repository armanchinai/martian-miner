//
// Created by wifir on 2026-02-25.
//

#ifndef LECTURE7_CAMERASYSTEM_H
#define LECTURE7_CAMERASYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

/*
 * CameraSystem
 *
 * Responsible for positioning camera entities so they follow the player.
 */
class CameraSystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities)
    {
        Entity* playerEntity = nullptr;

        // Find the player entity.
        for (auto& entity : entities)
        {
            if (entity->hasComponent<PlayerTag>())
            {
                playerEntity = entity.get();
                break;
            }
        }

        // If no player exists, nothing to follow
        if (!playerEntity) return;

        // Get player's position from transform.
        auto& playerT = playerEntity->getComponent<Transform>();

        // Update camera entity
        for (auto& entity : entities)
        {
            if (entity->hasComponent<Camera>())
            {
                auto& camera = entity->getComponent<Camera>();

                // Center camera on the player
                camera.view.x = playerT.position.x - camera.view.w / 2;
                camera.view.y = playerT.position.y - camera.view.h / 2;

                // Vertically and horizontally clamp camera within world bounds
                if (camera.view.x < 0)
                {
                    camera.view.x = 0;
                }
                if (camera.view.x > camera.worldWidth - camera.view.w)
                {
                    camera.view.x = camera.worldWidth - camera.view.w;
                }

                if (camera.view.y < 0)
                {
                    camera.view.y = 0;
                }
                if (camera.view.y > camera.worldHeight - camera.view.h)
                {
                    camera.view.y = camera.worldHeight - camera.view.h;
                }
            }
        }
    }
};

#endif //LECTURE7_CAMERASYSTEM_H