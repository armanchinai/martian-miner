//
// Created by wifir on 2026-01-21.
//

#ifndef LECTURE3_MOVEMENTSYSTEM_H
#define LECTURE3_MOVEMENTSYSTEM_H
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include "Entity.h"
#include "Component.h"

/* MovementSystem
 *
 * Responsible for updating entity positions based on their velocity.
 */
class MovementSystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const float deltaTime)
    {
        for (auto& entity : entities)
        {
            // Only process entities with position and velocity
            if (entity->hasComponent<Transform>() && entity->hasComponent<Velocity>())
            {
                auto& t = entity->getComponent<Transform>();
                auto& v = entity->getComponent<Velocity>();

                // Only update prevPosition if movement exceeds threshold
                float updateThreshold = 4;
                if (std::fabs(t.prevPosition.x - t.position.x) > updateThreshold || std::fabs(t.prevPosition.y - t.position.y) > updateThreshold) {
                    t.prevPosition = t.position;
                }

                // Normalize direction
                v.direction.normalize();
                // Compute velocity vector
                Vector2D velocityVector = v.direction * v.magnitude;
                // Apply movement
                t.position += velocityVector * deltaTime;
            }
        }
    }
};

#endif //LECTURE3_MOVEMENTSYSTEM_H