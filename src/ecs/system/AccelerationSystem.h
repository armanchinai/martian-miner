//
// Created by arman on 2026-03-17.
//

#ifndef MARTIAN_MINER_ACCELERATIONSYSTEM_H
#define MARTIAN_MINER_ACCELERATIONSYSTEM_H
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class AccelerationSystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities)
    {
        for (auto& entity : entities)
        {
            if (!entity->hasComponent<Acceleration>() ||
                !entity->hasComponent<PhysicsObject>() ||
                !entity->hasComponent<ForceInput>())
                continue;

            auto& acc = entity->getComponent<Acceleration>();
            auto& phys = entity->getComponent<PhysicsObject>();
            auto& input = entity->getComponent<ForceInput>();

            // get total force
            Vector2D totalForce{0.0f, 0.0f};

            float cosA = std::cos((phys.angle * std::numbers::pi)/180);
            float sinA = std::sin((phys.angle * std::numbers::pi)/180);

            Vector2D rotatedInput;
            rotatedInput.x = input.inputPositional.x * cosA - input.inputPositional.y * sinA;
            rotatedInput.y = input.inputPositional.x * sinA + input.inputPositional.y * cosA;

            totalForce += rotatedInput;

            if (phys.isGravityEnabled)
            {
                totalForce.y += phys.mass * phys.gravity;
            }

            // std::cout << "Input: " << input.inputPositional.x << " " << input.inputPositional.y << std::endl;
            // std::cout << "Rotational: " << rotatedInput.x<<" "<<rotatedInput.y<< std::endl;
            // std::cout << "TotalForce: " << totalForce.x << " " << totalForce.y << std::endl;

            // convert to acceleration
            Vector2D accelVec;

            accelVec.x = totalForce.x / phys.mass;
            accelVec.y = totalForce.y / phys.mass;

            // convert to magnitude and direction
            float mag = std::sqrt(accelVec.x * accelVec.x + accelVec.y * accelVec.y);

            acc.magnitude = mag;

            if (mag > 0.0001f)
            {
                acc.direction.x = accelVec.x / mag;
                acc.direction.y = accelVec.y / mag;
            }
            else
            {
                acc.direction = {0.0f, 0.0f};
            }
        }
    }
};

#endif //MARTIAN_MINER_ACCELERATIONSYSTEM_H