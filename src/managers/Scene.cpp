//
// Created by wifir on 2026-02-25.
//

#include "Scene.h"

#include "AssetManager.h"

Scene::Scene(const char* sceneName, const char* mapPath, int windowWidth, int windowHeight)
{
    name = sceneName;
    world.getMap().load(mapPath, TextureManager::load("../assets/spriteSheet.png"));
    for (const Collider& collider : world.getMap().colliders)
    {
        auto& e = world.createEntity();
        e.addComponent<Transform>(Vector2D(collider.rect.x, collider.rect.y), 0.0f, 1.0f);
        auto& c = e.addComponent<Collider>("wall");
        c.rect.x = collider.rect.x;
        c.rect.y = collider.rect.y;
        c.rect.w = collider.rect.w;
        c.rect.h = collider.rect.h;

        SDL_Texture* colTex = TextureManager::load("../assets/spriteSheet.png");
        SDL_FRect colTexSrc = {1, 33, 30, 30};
        SDL_FRect colTexDst = {c.rect.x, c.rect.y, c.rect.w, c.rect.h};
        e.addComponent<Sprite>(colTex, colTexSrc, colTexDst);
    }

    for (Vector2D& pos : world.getMap().itemLocations)
    {
        auto& item(world.createEntity());
        const auto& itemT = item.addComponent<Transform>(pos, 0.0f, 1.0f);
        SDL_Texture* itemTex = TextureManager::load("../assets/coin.png");
        SDL_FRect itemSrc {0, 0, 32, 32};
        SDL_FRect itemDst {itemT.position.x, itemT.position.y, 32, 32};
        item.addComponent<Sprite>(itemTex, itemSrc, itemDst);
        auto& itemCol = item.addComponent<Collider>("item");
        itemCol.rect.w = itemDst.w;
        itemCol.rect.h = itemDst.h;
        itemCol.rect.x = itemT.position.x;
        itemCol.rect.y = itemT.position.y;
    }

    auto& camera(world.createEntity());
    SDL_FRect camView;
    camView.w = windowWidth;
    camView.h = windowHeight;
    camera.addComponent<Camera>(camView, world.getMap().width * 32, world.getMap().height * 32);

    auto& player(world.createEntity());
    player.addComponent<PlayerTag>();

    const auto& playerT = player.addComponent<Transform>(Vector2D(0.0f,0.0f), 0.0f, 1.0f);

    player.addComponent<Velocity>(Vector2D(0.0f, 0.0f), 200.0f);
    player.addComponent<Acceleration>(Vector2D(0.0f, 0.0f));
    player.addComponent<PhysicsObject>();
    player.addComponent<ForceInput>();

    Animation anim = AssetManager::getAnimation("player");
    player.addComponent<Animation>(anim);

    SDL_Texture* tex = TextureManager::load("../assets/animations/box_anim.png");
    SDL_FRect playerSrc = anim.clips[anim.currentClip].frameIndices[0];
    SDL_FRect playerDst {playerT.position.x, playerT.position.y, 64, 64};
    player.addComponent<Sprite>(tex, playerSrc, playerDst);

    auto& playerCol = player.addComponent<Collider>("player");
    playerCol.rect.w = playerDst.w;
    playerCol.rect.h = playerDst.h;

    auto& spawner(world.createEntity());
    auto spawnerT = spawner.addComponent<Transform>(Vector2D(windowWidth/2, windowHeight - 5), 0.0f, 1.0f);
    spawner.addComponent<TimedSpawner>(2.0f, [this, spawnerT]
    {
        auto& bird(world.createDeferredEntity());
        bird.addComponent<ProjectileTag>();
        bird.addComponent<Transform>(Vector2D(spawnerT.position.x, spawnerT.position.y), 0.0f, 1.0f);
        bird.addComponent<Velocity>(Vector2D(0, -1), 75.0f);

        Animation birdAnim = AssetManager::getAnimation("enemy");
        bird.addComponent<Animation>(birdAnim);

        SDL_Texture* tex = TextureManager::load("../assets/animations/bird_anim.png");
        SDL_FRect src {0, 0, 32, 32};
        SDL_FRect dst {spawnerT.position.x, spawnerT.position.y, 32, 32};
        bird.addComponent<Sprite>(tex, src, dst);

        auto c = bird.addComponent<Collider>("projectile", dst);
    });

    auto& sceneState(world.createEntity());
    sceneState.addComponent<SceneState>();
}
