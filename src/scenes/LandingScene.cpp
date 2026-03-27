//
// Created by qthompson on 2026-03-24.
//

#include "LandingScene.h"

#include "managers/AssetManager.h"

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

    auto& player(world.createEntity());
    player.addComponent<PlayerTag>();

    const auto& playerT = player.addComponent<Transform>(Vector2D(0.0f,0.0f), 0.0f, 1.0f);

    player.addComponent<Velocity>(Vector2D(0.0f, 0.0f), 200.0f);
    player.addComponent<Acceleration>(Vector2D(0.0f, 0.0f));
    player.addComponent<PhysicsObject>();
    player.addComponent<ForceInput>();

    Animation anim = AssetManager::getAnimation("player");
    player.addComponent<Animation>(anim);

    SDL_Texture* tex = TextureManager::load("../assets/animations/lander1.png");
    SDL_FRect playerSrc = anim.clips[anim.currentClip].frameIndices[0];
    SDL_FRect playerDst {playerT.position.x, playerT.position.y, 64, 64};
    player.addComponent<Sprite>(tex, playerSrc, playerDst);

    auto& playerCol = player.addComponent<Collider>("player");
    playerCol.rect.w = playerDst.w;
    playerCol.rect.h = playerDst.h;
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
        SDL_FColor skyColour = {0.1f, 0.0f, 0.2f, 1.0f};
        SDL_FColor groundColour = {0.95f, 0.80f, 0.55f, 1.0f};
        SDL_FColor underGroundColour = {0.40f, 0.30f, 0.20f, 1.0f};

        auto& playerTransform = playerEntity->getComponent<Transform>();
        auto& camera = cameraEntity->getComponent<Camera>();

        if (const float heightPercent = playerTransform.position.y / camera.worldHeight; heightPercent < 0.20f)
        {
            // 0.00 → 0.10 : sky → ground
            float blend = heightPercent / 0.20f;  // remap to 0 → 1
            return lerp(skyColour, groundColour, blend);
        }
        else if (heightPercent < 0.90f)
        {
            // 0.10 → 0.90 : ground → underground
            float blend = (heightPercent - 0.20f) / 0.70f;  // remap to 0 → 1
            return lerp(groundColour, underGroundColour, blend);
        }
        else
        {
            // 0.90 → 1.00 : underground → deeper underground (or just underground)
            float blend = (heightPercent - 0.90f) / 0.10f;
            return lerp(underGroundColour, underGroundColour, blend); // effectively just underground
        }

    }
    return {0.0f, 0.0f, 0.0f, 1.0f};
}
