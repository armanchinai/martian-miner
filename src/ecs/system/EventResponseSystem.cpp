//
// Created by wifir on 2026-03-30.
//

#include "EventResponseSystem.h"

#include "World.h"

EventResponseSystem::EventResponseSystem(World &world) {
    world.getEventManager().subscribe(
        [this, &world](const BaseEvent& e) {
            if (e.type == EventType::Collision) {
                return;
            }

            const auto& collision = static_cast<const CollisionEvent&>(e);
            onCollision(collision, "item", world);
            onCollision(collision, "wall", world);
            onCollision(collision, "projectile", world);
        }
    );
}
