//
// Created by qthompson on 2026-03-24.
//

#include "LandingScene.h"
#include "managers/AssetManager.h"
#include "utils/Collision.h"

LandingScene::LandingScene(const char* name, const int windowWidth, const int windowHeight) :
Scene(name, windowWidth, windowHeight, "../assets/martianValleys2.tmx", "../assets/mars_tileset_w_background.png",
    {
        {8, 8, 16, 16},
        {8 + 32, 8, 16, 16},
        {8 + 32 * 2, 8, 16, 16},
        {8 + 32 * 3, 8, 16, 16},
        {8 + 32 * 4, 8, 16, 16},
        {8, 8 + 32, 16, 16},
        {8 + 32, 8 + 32, 16, 16},
        {8 + 32 * 2, 8 + 32, 16, 16},
        {8 + 32 * 3, 8 + 32, 16, 16},
        {8 + 32 * 4, 8 + 32, 16, 16},
        {8, 8 + 32 * 2, 16, 16},
        {8 + 32, 8 + 32 * 2, 16, 16},
        {8 + 32 * 2, 8 + 32 * 2, 16, 16},
        {8 + 32 * 3, 8 + 32 * 2, 16, 16},
        {8 + 32 * 4, 8 + 32 * 2, 16, 16},
        {8, 8 + 32 * 3, 16, 16},
        {8 + 32, 8 + 32 * 3, 16, 16},
        {8 + 32 * 2, 8 + 32 * 3, 16, 16},
        {8 + 32 * 3, 8 + 32 * 3, 16, 16},
        {8 + 32 * 4, 8 + 32 * 3, 16, 16},
        {8, 8 + 32 * 4, 16, 16},
        {8 + 32, 8 + 32 * 4, 16, 16},
        {8 + 32 * 2, 8 + 32 * 4, 16, 16},
        {8 + 32 * 3, 8 + 32 * 4, 16, 16},
        {8 + 32 * 4, 8 + 32 * 4, 16, 16},
        {8, 8 + 32 * 5, 16, 16},
        {8 + 32, 8 + 32 * 5, 16, 16},
        {8 + 32 * 2, 8 + 32 * 5, 16, 16},
        {8 + 32 * 3, 8 + 32 * 5, 16, 16},
        {8 + 32 * 4, 8 + 32 * 5, 16, 16},
    })
{
    for (const SDL_FRect& rect : world.getMap().landingZones)
    {
        auto& e = world.createEntity();
        e.addComponent<Transform>(Vector2D(rect.x, rect.y), 0.0f, 1.0f);
        auto& c = e.addComponent<Collider>("landingZone");
        c.rect.x = rect.x;
        c.rect.y = rect.y;
        c.rect.w = rect.w;
        c.rect.h = rect.h;

        SDL_Texture* colTex = TextureManager::load("../assets/gizmos.png");
        SDL_FRect colTexSrc = {33, 1, 30, 30};
        SDL_FRect colTexDst = {rect.x, rect.y, rect.w, rect.h};
        e.addComponent<Sprite>(colTex, colTexSrc, colTexDst);
    }

    auto& leftBarrier(world.createEntity());
    leftBarrier.addComponent<Transform>(Vector2D(-20.f, 0), 0.0f, 1.0f);
    auto& lCol = leftBarrier.addComponent<Collider>("barrier");
    lCol.rect.w = 20.0f;
    lCol.rect.h = world.getMap().height * world.getMap().tileHeight;

    auto& rightBarrier(world.createEntity());
    rightBarrier.addComponent<Transform>(Vector2D(world.getMap().width * world.getMap().tileWidth, 0), 0.0f, 1.0f);
    auto& rCol = rightBarrier.addComponent<Collider>("barrier");
    rCol.rect.w = 20.0f;
    rCol.rect.h = world.getMap().height * world.getMap().tileHeight;

    auto& topBarrier(world.createEntity());
    topBarrier.addComponent<Transform>(Vector2D(0, -21), 0.0f, 1.0f);
    auto& tCol = topBarrier.addComponent<Collider>("barrier");
    tCol.rect.h = 20.0f;
    tCol.rect.w = world.getMap().width * world.getMap().tileWidth;

    auto& bottomBarrier(world.createEntity());
    bottomBarrier.addComponent<Transform>(Vector2D(0, world.getMap().height * world.getMap().tileHeight + 1), 0.0f, 1.0f);
    auto& bCol = bottomBarrier.addComponent<Collider>("barrier");
    bCol.rect.h = 20.0f;
    bCol.rect.w = world.getMap().width * world.getMap().tileWidth;

    auto& player(world.createEntity());
    player.addComponent<PlayerTag>();

    const auto& playerT = player.addComponent<Transform>(Vector2D(world.getMap().width * world.getMap().tileWidth / 2,10.0f), 0.0f, 1.0f);

    player.addComponent<Velocity>(Vector2D(0.0f, 1.0f), 200.0f);
    player.addComponent<Acceleration>(Vector2D(0.0f, 0.0f));
    auto& physObj = player.addComponent<PhysicsObject>();
    physObj.angle = 0.0f;
    player.addComponent<ForceInput>();

    AssetManager::loadAnimation("player", "../assets/animations/lander_animations.xml");
    AssetManager::loadAnimation("explosion", "../assets/animations/explosion_animations.xml");

    Animation anim = AssetManager::getAnimation("player");
    player.addComponent<Animation>(anim);

    SDL_Texture* tex = TextureManager::load("../assets/animations/lander1.png");
    SDL_FRect playerSrc = anim.clips[anim.currentClip].frameIndices[0];
    SDL_FRect playerDst {playerT.position.x, playerT.position.y, 64, 64};
    player.addComponent<Sprite>(tex, playerSrc, playerDst);

    auto& playerCol = player.addComponent<Collider>("player");
    playerCol.rect.w = playerDst.w;
    playerCol.rect.h = playerDst.h;

    // Landing Zone Listener
    world.getEventManager().subscribe([&](const BaseEvent& e) {
        if (e.type != EventType::Collision) {
            return;
        }

        const auto collisionEvent = dynamic_cast<const CollisionEvent &>(e);

        Entity* entity = nullptr;
        Entity* other = nullptr;

        if (!Collision::getCollisionParticipants(collisionEvent, "player", "landingZone", entity, other))
        {
            return;
        }

        if (collisionEvent.state == CollisionState::Enter)
        {
            if (entity->hasComponent<PlayerTag>())
            {
                auto& playerTag = entity->getComponent<PlayerTag>();
                playerTag.withinLandingZone = true;
            }
        }
        else if (collisionEvent.state == CollisionState::Exit)
        {
            if (entity->hasComponent<PlayerTag>())
            {
                auto& playerTag = entity->getComponent<PlayerTag>();
                playerTag.withinLandingZone = false;
            }
        }
    });

    // Barrier/Wall Listener
    world.getEventManager().subscribe([&](const BaseEvent& e) {
        if (e.type != EventType::Collision) {
            return;
        }

        const auto collisionEvent = dynamic_cast<const CollisionEvent &>(e);

        Entity* entity = nullptr;
        Entity* other = nullptr;

        bool isWall = false;
        if (Entity* wall = nullptr; Collision::getCollisionParticipants(collisionEvent, "player", "wall", entity, wall))
        {
            other = wall;
            isWall = true;
        }
        else if (Entity* barrier = nullptr; Collision::getCollisionParticipants(collisionEvent, "player", "barrier", entity, barrier))
        {
            other = barrier;
        }
        else
        {
            return;
        }

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

        if (isWall)
        {
            auto& tag = entity->getComponent<PlayerTag>();
            if (!tag.withinLandingZone) {
                world.getEventManager().emit(PlayerActionEvent{entity, PlayerAction::Death});
            } else if (int angle = (static_cast<int>(t.rotation) % 360 + 360) % 360; (angle > 15 && angle < 345) || v.magnitude > 100.0f) {
                world.getEventManager().emit(PlayerActionEvent{entity, PlayerAction::Death});
            }
        }
    });

    // Player Death
    world.getEventManager().subscribe([&](const BaseEvent& e) {
        if (e.type != EventType::PlayerAction)
        {
            return;
        }

        const auto& actionEvent = static_cast<const PlayerActionEvent&>(e);

        if (actionEvent.action != PlayerAction::Death)
        {
            return;
        }

        auto &playerExplosion (world.createDeferredEntity());

        Animation explosionAnim = AssetManager::getAnimation("explosion");
        playerExplosion.addComponent<Animation>(explosionAnim);
        auto explosionT = playerExplosion.addComponent<Transform>(Vector2D(playerT.position.x, playerT.position.y), 0.0f, 1.0f);

        SDL_Texture* explosionTex = TextureManager::load("../assets/animations/explosion_anim.png");

        SDL_FRect explosionSrc = explosionAnim.clips[explosionAnim.currentClip].frameIndices[0];
        SDL_FRect explosionDst {explosionT.position.x, explosionT.position.y, 64, 64};
        playerExplosion.addComponent<Sprite>(explosionTex, explosionSrc, explosionDst);
    });
}

SDL_FColor lerp(const SDL_FColor& a, const SDL_FColor& b, float t)
{
    return {
        a.r + (b.r - a.r) * t,
        a.g + (b.g - a.g) * t,
        a.b + (b.b - a.b) * t,
        1.0f
    };
}

SDL_FColor LandingScene::getBackgroundColour()
{
    for (auto& entity : world.getEntities())
    {
        if (playerEntity && cameraEntity)
        {
            break;
        }

        if (entity->hasComponent<Camera>())
        {
            cameraEntity = entity.get();
        }
        else if (entity->hasComponent<PlayerTag>())
        {
            playerEntity = entity.get();
        }
    }

    if (playerEntity && cameraEntity)
    {
        constexpr SDL_FColor skyColour = {0.1f, 0.0f, 0.2f, 1.0f};
        constexpr SDL_FColor groundColour = {0.95f, 0.80f, 0.55f, 1.0f};
        constexpr SDL_FColor underGroundColour = {0.16f, 0.0f, 0.01f, 1.0f};

        const auto& playerTransform = playerEntity->getComponent<Transform>();
        const auto& camera = cameraEntity->getComponent<Camera>();

        if (const float heightPercent = playerTransform.position.y / camera.worldHeight; heightPercent < 0.20f)
        {
            const float blend = heightPercent / 0.20f;
            return lerp(skyColour, groundColour, blend);
        }
        else {
            if (heightPercent < 0.90f)
            {
                const float blend = (heightPercent - 0.20f) / 0.70f;
                return lerp(groundColour, underGroundColour, blend);
            }
            const float blend = (heightPercent - 0.90f) / 0.10f;
            return lerp(underGroundColour, underGroundColour, blend);
        }
    }
    return {0.0f, 0.0f, 0.0f, 1.0f};
}
