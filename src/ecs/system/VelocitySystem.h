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

class VelocitySystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime)
    {
        for (auto& entity : entities)
        {
            if (!entity->hasComponent<Velocity>() ||
                !entity->hasComponent<Acceleration>())
                continue;

            auto& vel = entity->getComponent<Velocity>();
            auto& acc = entity->getComponent<Acceleration>();

            // set up vectors
            Vector2D velVec{
                vel.direction.x * vel.magnitude,
                vel.direction.y * vel.magnitude
            };

            Vector2D accVec{
                acc.direction.x * acc.magnitude,
                acc.direction.y * acc.magnitude
            };

            // integrate
            velVec.x += accVec.x * deltaTime;
            velVec.y += accVec.y * deltaTime;

            // convert back to mag/dir
            float mag = std::sqrt(velVec.x * velVec.x + velVec.y * velVec.y);

            vel.magnitude = mag;

            if (mag > 0.0001f)
            {
                vel.direction.x = velVec.x / mag;
                vel.direction.y = velVec.y / mag;
            }
            else
            {
                vel.direction = {0.0f, 0.0f};
            }
        }
    }
};

#endif //MARTIAN_MINER_VELOCITYSYSTEM_H