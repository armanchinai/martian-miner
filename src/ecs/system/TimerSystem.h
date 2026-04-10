//
// Created by wifir on 2026-04-08.
//

#ifndef MARTIAN_MINER_TIMERSYSTEM_H
#define MARTIAN_MINER_TIMERSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class TimerSystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const float deltaTime)
    {
        for (auto& entity : entities)
        {
            if (entity->hasComponent<Timer>())
            {
                auto& timer = entity->getComponent<Timer>();
                timer.counter -= deltaTime;

                if (timer.counter <= 0)
                {
                    timer.counter = timer.interval;
                    timer.timerCallback();
                    if (timer.runOnce) {
                        entity->deactivateComponent<Timer>();
                    }
                }
            }
        }
    }
};

#endif //MARTIAN_MINER_TIMERSYSTEM_H
