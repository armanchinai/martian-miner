//
// Created by wifir on 2026-03-30.
//

#include "EventResponseSystem.h"

#include "World.h"
#include "Game.h"

EventResponseSystem::EventResponseSystem(World &world) {
    world.getEventManager().subscribe(
        [this, &world](const BaseEvent& e) {
            if (e.type != EventType::Collision) {
                return;
            }

            const auto& collision = static_cast<const CollisionEvent&>(e);
            onCollision(collision, "player", "landingZone", world);
            onCollision(collision, "player", "wall", world);
            onCollision(collision, "player", "barrier", world);
        }
    );

    world.getEventManager().subscribe(
    [this, &world](const BaseEvent& e)
    {
        if (e.type != EventType::PlayerAction)
            return;

        const auto& actionEvent = static_cast<const PlayerActionEvent&>(e);

        if (actionEvent.action == PlayerAction::Death)
        {
            onPlayerAction(actionEvent.action, [](Entity* player, PlayerAction a) {});
        }
    }
);
}

void EventResponseSystem::onCollision(const CollisionEvent& e, const char* entityTag, const char* otherTag, World& world)
{
    Entity* entity = nullptr;
    Entity* other = nullptr;

    if (!(getCollisionEntities(e, entityTag, otherTag, entity, other)))
    {
        return;
    }

    if ((std::string(entityTag) == "player" && std::string(otherTag) == "barrier") || (std::string(entityTag) == "player" && std::string(otherTag) == "wall"))
    {
        auto& t = entity->getComponent<Transform>();
        auto& v = entity->getComponent<Velocity>();
        auto& playerRect = entity->getComponent<Collider>().rect;
        auto& wallRect   = other->getComponent<Collider>().rect;

        const float playerCenterX = playerRect.x + playerRect.w  * 0.5f;
        const float playerCenterY = playerRect.y + playerRect.h * 0.5f;

        const float wallCenterX = wallRect.x + wallRect.w  * 0.5f;
        const float wallCenterY = wallRect.y + wallRect.h * 0.5f;

        const float playerHalfW = playerRect.w  * 0.5f;
        const float playerHalfH = playerRect.h * 0.5f;

        const float wallHalfW = wallRect.w  * 0.5f;
        const float wallHalfH = wallRect.h * 0.5f;

        const float dx = playerCenterX - wallCenterX;
        const float dy = playerCenterY - wallCenterY;

        const float overlapX = playerHalfW + wallHalfW - std::abs(dx);

        if (const float overlapY = playerHalfH + wallHalfH - std::abs(dy); overlapX > 0.0f && overlapY > 0.0f)
        {
            if (overlapX < overlapY)
            {
                if (dx > 0) {
                    t.position.x += overlapX;
                    v.magnitude = std::sqrt((v.magnitude*v.magnitude) - (v.magnitude*v.direction.x)*(v.magnitude*v.direction.x));
                    v.direction.x = 0;
                    if (v.direction.y < 0) {
                        v.direction.y = -1;
                    } else {
                        v.direction.y = 1;
                    }
                }
                else {
                    t.position.x -= overlapX;
                    v.magnitude = std::sqrt((v.magnitude*v.magnitude) - (v.magnitude*v.direction.x)*(v.magnitude*v.direction.x));
                    v.direction.x = 0;
                    if (v.direction.y < 0) {
                        v.direction.y = -1;
                    } else {
                        v.direction.y = 1;
                    }
                }
            }
            else
            {
                if (dy > 0) {
                    t.position.y += overlapY;
                    v.magnitude = std::sqrt((v.magnitude*v.magnitude) - (v.magnitude*v.direction.y)*(v.magnitude*v.direction.y));
                    v.direction.y = 0;
                    if (v.direction.x < 0) {
                        v.direction.x = -1;
                    } else {
                        v.direction.x = 1;
                    }
                }
                else {
                    t.position.y -= overlapY;
                    v.magnitude = std::sqrt((v.magnitude*v.magnitude) - (v.magnitude*v.direction.y)*(v.magnitude*v.direction.y));
                    v.direction.y = 0;
                    v.direction.x = 0;
                    v.magnitude = 0;
                }
            }

            playerRect.x = t.position.x;
            playerRect.y = t.position.y;
        }
    }

    if (std::string(entityTag) == "player" && std::string(otherTag) == "wall") {
        auto& tag = entity->getComponent<PlayerTag>();
        if (!tag.withinLandingZone) {
            world.getEventManager().emit(PlayerActionEvent{entity, PlayerAction::Death});
        }
    }

    if (std::string(entityTag) == "player" && std::string(otherTag) == "landingZone")
    {
        auto& t = entity->getComponent<Transform>();
        auto& v = entity->getComponent<Velocity>();
        int angle = ((int)t.rotation % 360 + 360) % 360;

        if ((angle > 15 && angle < 345) || v.magnitude > 100.0f) {
            world.getEventManager().emit(PlayerActionEvent{entity, PlayerAction::Death});
        }

        if (e.state == CollisionState::Enter)
        {
            if (entity->hasComponent<PlayerTag>())
            {
                auto& playerTag = entity->getComponent<PlayerTag>();
                playerTag.withinLandingZone = true;
            }
        }
        else if (e.state == CollisionState::Exit)
        {
            if (entity->hasComponent<PlayerTag>())
            {
                auto& playerTag = entity->getComponent<PlayerTag>();
                playerTag.withinLandingZone = false;
            }
        }
    }
}

bool EventResponseSystem::getCollisionEntities(
    const CollisionEvent& e,
    const char* entityTag,
    const char* otherTag,
    Entity*& entity,
    Entity*& other) {
    if (e.entityA == nullptr || e.entityB == nullptr)
    {
        return false;
    }

    if (!(e.entityA->hasComponent<Collider>() && e.entityB->hasComponent<Collider>()))
    {
        return false;
    }

    auto& colA = e.entityA->getComponent<Collider>();
    auto& colB = e.entityB->getComponent<Collider>();

    if (colA.tag == entityTag && colB.tag == otherTag)
    {
        entity = e.entityA;
        other = e.entityB;
    }
    else if (colB.tag == entityTag && colA.tag == otherTag)
    {
        entity = e.entityB;
        other = e.entityA;
    }

    return entity && other;
}

void EventResponseSystem::onPlayerAction(
    const PlayerAction& action,
    const std::function<void(Entity* player, PlayerAction a)>& callback
) {
    // Here we just call the callback if the action is Death
    if (action == PlayerAction::Death) {
        std::cout << "Death" << std::endl;
        callback(nullptr, action); // nullptr because you can pass the player entity if needed
    }
}