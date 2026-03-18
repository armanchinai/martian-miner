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

class MovementSystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const float deltaTime)
    {
        for (auto& entity : entities)
        {
            if (entity->hasComponent<Transform>() && entity->hasComponent<Velocity>())
            {
                auto& t = entity->getComponent<Transform>();
                auto& v = entity->getComponent<Velocity>();

                float updateThreshold = 4;
                if (std::fabs(t.prevPosition.x - t.position.x) > updateThreshold || std::fabs(t.prevPosition.y - t.position.y) > updateThreshold) {
                    t.prevPosition = t.position;
                }

                v.direction.normalize();
                Vector2D velocityVector = v.direction * v.magnitude;
                t.position += velocityVector * deltaTime;
            }
        }
    }
};

#endif //LECTURE3_MOVEMENTSYSTEM_H