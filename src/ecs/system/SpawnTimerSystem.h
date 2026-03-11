//
// Created by wifir on 2026-02-25.
//

#ifndef LECTURE8_SPAWNTIMERSYSTEM_H
#define LECTURE8_SPAWNTIMERSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class SpawnTimerSystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const float deltaTime)
    {
        for (auto& entity : entities)
        {
            if (entity->hasComponent<TimedSpawner>())
            {
                auto& spawner = entity->getComponent<TimedSpawner>();
                spawner.timer -= deltaTime;

                if (spawner.timer <= 0)
                {
                    spawner.timer = spawner.spawnInterval;
                    spawner.spawnCallback();
                }
            }
        }
    }
};

#endif //LECTURE8_SPAWNTIMERSYSTEM_H