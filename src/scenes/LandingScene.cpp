//
// Created by qthompson on 2026-03-24.
//

#include "LandingScene.h"

#include "managers/AssetManager.h"

LandingScene::LandingScene(const char* name, const int windowWidth, const int windowHeight) :
Scene(name, windowWidth, windowHeight, "../assets/martianValleys.tmx", "../assets/mars_tileset.png",
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
