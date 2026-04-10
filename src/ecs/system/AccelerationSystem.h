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

/*
 * AccelerationSystem
 *
 * This system uses Newtonian physics to calculate the acceleration of entities based on the aggregation of forces
 * through unique components, as well as physics properties stored in the PhysicsObject component.
 *
 * Required Components:
 * - Acceleration: stores magnitude and direction of acceleration
 * - PhysicsObject: stores mass, angle, gravity, etc.
 * - ForceInput: stores user or AI input (rotation + thrust)
 * - Transform: used to sync rotation visually
 */
class AccelerationSystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, float deltaTime)
    {
        for (auto& entity : entities)
        {
            // Skip entities that don't have required components
            if (!entity->hasComponent<Acceleration>() ||
                !entity->hasComponent<PhysicsObject>() ||
                !entity->hasComponent<ForceInput>())
                continue;

            // Get required components
            auto& t = entity->getComponent<Transform>();
            auto& acc = entity->getComponent<Acceleration>();
            auto& phys = entity->getComponent<PhysicsObject>();
            auto& input = entity->getComponent<ForceInput>();

            // Calculate total force
            Vector2D totalForce{0.0f, 0.0f};

            // Convert angle to radians.
            float cosA = std::cos((phys.angle * std::numbers::pi)/180);
            float sinA = std::sin((phys.angle * std::numbers::pi)/180);

            // Create rotation matrix to calculate new basis vectors for force direction.
            Vector2D rotatedInput;
            rotatedInput.x = - input.inputPositional.y * sinA;
            rotatedInput.y = + input.inputPositional.y * cosA;

            totalForce += rotatedInput;

            // Apply gravity
            if (phys.isGravityEnabled)
            {
                totalForce.y += phys.mass * phys.gravity;
            }

            // Convert force into acceleration.
            Vector2D accelVec;

            accelVec.x = totalForce.x / phys.mass;
            accelVec.y = totalForce.y / phys.mass;

            // Separate force into magnitude and direction
            float mag = std::sqrt(accelVec.x * accelVec.x + accelVec.y * accelVec.y);

            acc.magnitude = mag;

            // Update rotational angle based on x component of input force.
            phys.angle += input.inputPositional.x * deltaTime;
            t.rotation = phys.angle;

            // Normalize direction vector.
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