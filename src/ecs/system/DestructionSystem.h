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
                if (t.position.x > camera.view.x + camera.view.w ||
                    t.position.y > camera.view.y + camera.view.h ||
                    t.position.x < camera.view.x ||
                    t.position.y < camera.view.y)
                {
                    entity->destroy();
                }
            }
        }
    }
};

#endif //LECTURE8_DESTRUCTIONSYSTEM_H