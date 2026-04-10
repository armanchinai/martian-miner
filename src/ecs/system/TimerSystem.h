//
// Created by wifir on 2026-04-08.
//

#ifndef MARTIAN_MINER_TIMERSYSTEM_H
#define MARTIAN_MINER_TIMERSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

/*
 * TimerSystem
 *
 * Responsible for updating timers and triggering timed callbacks.
 */
class TimerSystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const float deltaTime)
    {
        for (auto& entity : entities)
        {
            // Only process entities with a Timer component
            if (entity->hasComponent<Timer>())
            {
                auto& timer = entity->getComponent<Timer>();

                // Decrease timer counter
                timer.counter -= deltaTime;

                // Check if timer has expired
                if (timer.counter <= 0)
                {
                    // Reset timer for next interval
                    timer.counter = timer.interval;
                    // Execute the associated callback
                    timer.timerCallback();
                    // Disable the Timer component after execution if non-repeating.
                    if (timer.runOnce) {
                        entity->deactivateComponent<Timer>();
                    }
                }
            }
        }
    }
};

#endif //MARTIAN_MINER_TIMERSYSTEM_H
