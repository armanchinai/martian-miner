//
// Created by wifir on 2026-02-04.
//

#include "World.h"

#include "Game.h"

void collisionEventListener2 (const CollisionEvent collision)
{
    if (collision.entityA->hasComponent<Collider>() && collision.entityB->hasComponent<Collider>())
    {
        auto& colA = collision.entityA->getComponent<Collider>();
        auto& colB = collision.entityB->getComponent<Collider>();
        std::cout << "A collision occured between entityA (" << colA.tag << ") and entityB (" << colB.tag << ")." << std::endl;
    }
}

World::World()
{
    eventManager.subscribe([this](const CollisionEvent collision)
    {
        Entity* sceneStateEntity = nullptr;

        for (auto& entity : entities)
        {
            if (entity->hasComponent<SceneState>())
            {
                sceneStateEntity = entity.get();
                break;
            }
        }

        if (!sceneStateEntity) return;

        if (collision.entityA == nullptr || collision.entityB == nullptr) { return; }
        if (!(collision.entityA->hasComponent<Collider>() && collision.entityB->hasComponent<Collider>())) { return; }

        auto& colA = collision.entityA->getComponent<Collider>();
        auto& colB = collision.entityB->getComponent<Collider>();
        Entity* player = nullptr;
        Entity* item = nullptr;
        Entity* wall = nullptr;
        Entity* projectile = nullptr;
        if (colA.tag == "player" && colB.tag == "item")
        {
            player = collision.entityA;
            item = collision.entityB;
        }
        else if (colB.tag == "player" && colA.tag == "item")
        {
            player = collision.entityB;
            item = collision.entityA;
        }

        if (player && item)
        {
            item->destroy();
            auto& sceneState = sceneStateEntity->getComponent<SceneState>();
            sceneState.coinsCollected++;
            if (sceneState.coinsCollected > 1)
            {
                Game::onSceneChangeRequest("level2");
            }
        }

        if (colA.tag == "player" && colB.tag == "wall")
        {
            player = collision.entityA;
            wall = collision.entityB;
        }
        else if (colB.tag == "player" && colA.tag == "wall")
        {
            player = collision.entityB;
            wall = collision.entityA;
        }

        if (player && wall) {
            auto& t = player->getComponent<Transform>();
            auto& v = player->getComponent<Velocity>();
            auto& playerRect = player->getComponent<Collider>().rect;
            auto& wallRect   = wall->getComponent<Collider>().rect;

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
                        v.direction.y = 1;
                    }
                    else {
                        t.position.x -= overlapX;
                        v.magnitude = std::sqrt((v.magnitude*v.magnitude) - (v.magnitude*v.direction.x)*(v.magnitude*v.direction.x));
                        v.direction.x = 0;
                        v.direction.y = 1;
                    }
                }
                else
                {
                    if (dy > 0) {
                        t.position.y += overlapY;
                    }
                    else {
                        t.position.y -= overlapY;
                        v.direction.y = 0;
                    }
                }

                playerRect.x = t.position.x;
                playerRect.y = t.position.y;
            }
        }

        if (colA.tag == "player" && colB.tag == "projectile")
        {
            player = collision.entityA;
            projectile = collision.entityB;
        }
        else if (colB.tag == "player" && colA.tag == "projectile")
        {
            player = collision.entityB;
            projectile = collision.entityA;
        }

        if (projectile && player)
        {
            player->destroy();
            Game::onSceneChangeRequest("gameover");
        }
    });

    eventManager.subscribe(collisionEventListener2);
}
