//
// Created by wifir on 2026-02-25.
//

#ifndef LECTURE7_CAMERASYSTEM_H
#define LECTURE7_CAMERASYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class CameraSystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities)
    {
        Entity* playerEntity = nullptr;

        for (auto& entity : entities)
        {
            if (entity->hasComponent<PlayerTag>())
            {
                playerEntity = entity.get();
                break;
            }
        }

        if (!playerEntity) return;

        auto& playerT = playerEntity->getComponent<Transform>();

        for (auto& entity : entities)
        {
            if (entity->hasComponent<Camera>())
            {
                auto& camera = entity->getComponent<Camera>();
                camera.view.x = playerT.position.x - camera.view.w / 2;
                camera.view.y = playerT.position.y - camera.view.h / 2;

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