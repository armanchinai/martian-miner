//
// Created by wifir on 2026-04-06.
//

#include "MainMenuScene.h"

MainMenuScene::MainMenuScene(const char* name, int windowWidth, int windowHeight) :
Scene(name, windowWidth, windowHeight)
{
    auto& menu(world.createEntity());
    const auto menuTransform = menu.addComponent<Transform>(Vector2D(0.0f, 0.0f), 0.0f, 1.0f);

    SDL_Texture* panelTex = TextureManager::load("../assets/martian_miner_ruinsbg.png");
    SDL_FRect panelSrc {0, 0 , 2560.0f, 1920.0f};
    SDL_FRect panelDst {menuTransform.position.x, menuTransform.position.y , static_cast<float>(windowWidth), static_cast<float>(windowHeight)};
    menu.addComponent<Sprite>(panelTex, panelSrc, panelDst, RenderLayer::UI);

    auto& title(world.createEntity());
    const auto titleTransform = title.addComponent<Transform>(Vector2D((windowWidth/2) - 308.0f/2, 25.0f), 0.0f, 1.0f);

    SDL_Texture* titleTex = TextureManager::load("../assets/martian_miner_title.png");
    SDL_FRect titleSrc {0, 0 , 308.0f, 84.0f};
    SDL_FRect titleDst {titleTransform.position.x, titleTransform.position.y , titleSrc.w, titleSrc.h};
    title.addComponent<Sprite>(titleTex, titleSrc, titleDst, RenderLayer::UI);

    auto& startButton(world.createEntity());
    auto& startTransform = startButton.addComponent<Transform>(Vector2D((float) windowWidth/2 - 32, (float) windowHeight - 100), 0.0f, 1.0f);
    SDL_Texture *symbolsTex = TextureManager::load("../assets/ui_symbols.png");
    SDL_FRect cogSrc = {64*7, 0, 64, 64};
    SDL_FRect cogDst = {startTransform.position.x, startTransform.position.y, cogSrc.w, cogSrc.h};
    startButton.addComponent<Sprite>(symbolsTex, cogSrc, cogDst, RenderLayer::UI);
    startButton.addComponent<Collider>("ui", cogDst);
    auto& clickable = startButton.addComponent<Clickable>();
    clickable.onPressed = [&startTransform] {
        startTransform.scale = 0.75f;
    };

    clickable.onReleased = [this, &startTransform] {
        startTransform.scale = 1.0f;
        world.getEventManager().emit(SceneSwapEvent("game"));
    };

    clickable.onCancelled = [&startTransform] {
        startTransform.scale = 1.0f;
    };

    world.getEventManager().subscribe([&](const BaseEvent& e) {

    });
}

SDL_FColor MainMenuScene::getBackgroundColour() {
    return {0.05f, 0.0f, 0.08f};
}

void MainMenuScene::onMouseInteraction(const MouseInteractionEvent& mouseInteraction) {
    Scene::onMouseInteraction(mouseInteraction);
    if (!mouseInteraction.entity->hasComponent<Clickable>()) {
        return;
    }

    if (mouseInteraction.state == MouseInteractionState::Released) {
        world.getAudioEventQueue().push(std::make_unique<AudioEvent>("button"));
    }
}