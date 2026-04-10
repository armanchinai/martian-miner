//
// Created by arman on 2026-03-17.
//

#ifndef MARTIAN_MINER_VELOCITYSYSTEM_H
#define MARTIAN_MINER_VELOCITYSYSTEM_H
#include <cmath>
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

/*
 * VelocitySystem
 *
 * Responsible for integrating acceleration into velocity. Converts acceleration into changes in velocity over time.
 */
class VelocitySystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime)
    {
        for (auto& entity : entities)
        {
            // Only process entities with both Velocity and Acceleration
            if (!entity->hasComponent<Velocity>() ||
                !entity->hasComponent<Acceleration>())
                continue;

            auto& vel = entity->getComponent<Velocity>();
            auto& acc = entity->getComponent<Acceleration>();

            // Convert to vector form
            Vector2D velVec{
                vel.direction.x * vel.magnitude,
                vel.direction.y * vel.magnitude
            };

            Vector2D accVec{
                acc.direction.x * acc.magnitude,
                acc.direction.y * acc.magnitude
            };

            // Integrate acceleration
            velVec.x += accVec.x * deltaTime;
            velVec.y += accVec.y * deltaTime;

            // Convert back to magnitude + direction
            float mag = std::sqrt(velVec.x * velVec.x + velVec.y * velVec.y);

            vel.magnitude = mag;

            // Catch divide by zero errors, effectively no movement below magnitude threshold.
            if (mag > 0.0001f)
            {
                vel.direction.x = velVec.x / mag;
                vel.direction.y = velVec.y / mag;
            }
            else
            {
                vel.direction = {0.0f, 0.0f};
            }

            // Reset acceleration. These forces are applied for only this frame, and are recalculated in next frame.
            acc.magnitude = 0;
            acc.direction.x = 0;
            acc.direction.y = 0;
        }
    }
};

#endif //MARTIAN_MINER_VELOCITYSYSTEM_H