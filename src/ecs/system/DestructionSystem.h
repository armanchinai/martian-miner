//
// Created by wifir on 2026-02-25.
//

#ifndef LECTURE8_DESTRUCTIONSYSTEM_H
#define LECTURE8_DESTRUCTIONSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class DestructionSystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities)
    {
        Entity* cameraEntity = nullptr;
        for (auto& entity : entities)
        {
            if (entity->hasComponent<Camera>())
            {
                cameraEntity = entity.get();
                break;
            }
        }

        if (!cameraEntity) return;
        auto& camera = cameraEntity->getComponent<Camera>();

        for (auto& entity : entities)
        {
            if (entity->hasComponent<Transform>() && entity->hasComponent<ProjectileTag>())
            {
                auto& t = entity->getComponent<Transform>();
                if (t.position.x > camera.worldWidth ||
                    t.position.y > camera.worldHeight ||
                    t.position.x < 0 ||
                    t.position.y < 0)
                {
                    entity->destroy();
                }
            }
        }
    }
};

#endif //LECTURE8_DESTRUCTIONSYSTEM_H