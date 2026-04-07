//
// Created by wifir on 2026-04-06.
//

#include "MainMenuScene.h"

MainMenuScene::MainMenuScene(const char* name, int windowWidth, int windowHeight) :
Scene(name, windowWidth, windowHeight)
{
    auto& menu(world.createEntity());
    const auto menuTransform = menu.addComponent<Transform>(Vector2D(0.0f, 0.0f), 0.0f, 1.0f);

    SDL_Texture* panelTex = TextureManager::load("../assets/martian_miner_bg.png");
    SDL_FRect panelSrc {0, 0 , 1280.0f, 960.0f};
    SDL_FRect panelDst {menuTransform.position.x, menuTransform.position.y , static_cast<float>(windowWidth), static_cast<float>(windowHeight)};
    menu.addComponent<Sprite>(panelTex, panelSrc, panelDst, RenderLayer::UI);

    auto& title(world.createEntity());
    const auto titleTransform = title.addComponent<Transform>(Vector2D((windowWidth/2) - 308.0f/2, 50.0f), 0.0f, 1.0f);

    SDL_Texture* titleTex = TextureManager::load("../assets/martian_miner_title.png");
    SDL_FRect titleSrc {0, 0 , 308.0f, 84.0f};
    SDL_FRect titleDst {titleTransform.position.x, titleTransform.position.y , titleSrc.w, titleSrc.h};
    title.addComponent<Sprite>(titleTex, titleSrc, titleDst, RenderLayer::UI);

    auto& startButton(world.createEntity());
    auto& startTransform = startButton.addComponent<Transform>(Vector2D((float) windowWidth/2 - 32, (float) windowHeight - 150), 0.0f, 1.0f);
    SDL_Texture *symbolsTex = TextureManager::load("../assets/ui_symbols.png");
    SDL_FRect cogSrc = {64*7, 0, 64, 64};
    SDL_FRect cogDst = {startTransform.position.x, startTransform.position.y, cogSrc.w, cogSrc.h};
    startButton.addComponent<Sprite>(symbolsTex, cogSrc, cogDst, RenderLayer::UI);
    startButton.addComponent<Collider>("ui", cogDst);
    auto& clickable = startButton.addComponent<Clickable>();
    clickable.onPressed = [&startTransform] {
        startTransform.scale = 0.5f;
    };

    clickable.onReleased = [&startTransform] {
        startTransform.scale = 1.0f;

    };

    clickable.onCancelled = [&startTransform] {
        startTransform.scale = 1.0f;
    };

    //auto& settingsOverlay = createSettingsOverlay(windowWidth, windowHeight);
    //createCogButton(windowWidth, windowHeight, settingsOverlay);

    world.getEventManager().subscribe([&](const BaseEvent& e) {
        if (e.type != EventType::MouseInteraction) {
            return;
        }

        const auto mouseInteractionEvent = dynamic_cast<const MouseInteractionEvent&>(e);

        if (!mouseInteractionEvent.entity->hasComponent<Clickable>()) {
            return;
        }

        auto& clickable = mouseInteractionEvent.entity->getComponent<Clickable>();

        switch (mouseInteractionEvent.state) {
            case MouseInteractionState::Pressed:
                clickable.onPressed();
                break;
            case MouseInteractionState::Released:
                clickable.onReleased();
                break;
            case MouseInteractionState::Cancelled:
                clickable.onCancelled();
        }
    });
}

SDL_FColor MainMenuScene::getBackgroundColour() {
    return {0.05f, 0.0f, 0.08f};
}

Entity &MainMenuScene::createCogButton(int windowWidth, int windowHeight, Entity &overlay) {
    auto& cogButton = world.createEntity();
    auto& cogTransform = cogButton.addComponent<Transform>(Vector2D((float) windowWidth - 50, (float) windowHeight - 50), 0.0f, 1.0f);

    SDL_Texture *symbolsTex = TextureManager::load("../assets/ui_symbols.png");
    SDL_FRect cogSrc = {64*5, 0, 64, 64};
    SDL_FRect cogDst = {cogTransform.position.x, cogTransform.position.y, cogSrc.w, cogSrc.h};
    cogButton.addComponent<Sprite>(symbolsTex, cogSrc, cogDst, RenderLayer::UI);
    cogButton.addComponent<Collider>("ui", cogDst);
    auto& clickable = cogButton.addComponent<Clickable>();
    clickable.onPressed = [&cogTransform] {
        cogTransform.scale = 0.5f;
    };

    clickable.onReleased = [this, &overlay, &cogTransform] {
        cogTransform.scale = 1.0f;
        toggleSettingsOverlayVisibility(overlay);
    };

    clickable.onCancelled = [&cogTransform] {
        cogTransform.scale = 1.0f;
    };

    return overlay;
}

Entity &MainMenuScene::createSettingsOverlay(int windowWidth, int windowHeight) {
    auto &overlay(world.createEntity());
    SDL_Texture *panelTex = TextureManager::load("../assets/martian_miner_bg.png");
    SDL_FRect panelSrc {0, 0 , 1280.0f, 960.0f};
    SDL_FRect panelDst {(float)windowWidth/2 - panelSrc.w/2, (float)windowHeight/2 - panelSrc.h/2, panelSrc.w/2, panelSrc.h/2};
    overlay.addComponent<Transform>(Vector2D(0.0f, 0.0f), 0.0f, 1.0f);
    overlay.addComponent<Sprite>(panelTex, panelSrc, panelDst, RenderLayer::UI, false);
    createSettingsUIComponents(overlay);
    return overlay;
}

void MainMenuScene::createSettingsUIComponents(Entity &overlay) {
    if (!overlay.hasComponent<Children>()) {
        overlay.addComponent<Children>();
    }

    auto& overlayTransform = overlay.getComponent<Transform>();
    auto& overlaySprite = overlay.getComponent<Sprite>();

    float baseX = overlayTransform.position.x;
    float baseY = overlayTransform.position.y;

    auto& closeButton = world.createEntity();
    auto& closeTransform = closeButton.addComponent<Transform>(Vector2D(baseX + overlaySprite.dst.w - 40, baseY + 10), 0.0f, 1.0f);

    SDL_Texture *symbolsTex = TextureManager::load("../assets/ui_symbols.png");
    SDL_FRect closeSrc = {64*4, 0, 64, 64};
    SDL_FRect closeDst = {closeTransform.position.x, closeTransform.position.y, closeSrc.w, closeSrc.h};
    closeButton.addComponent<Sprite>(symbolsTex, closeSrc, closeDst, RenderLayer::UI, false);
    closeButton.addComponent<Collider>("ui", closeDst);
    auto& clickable = closeButton.addComponent<Clickable>();
    clickable.onPressed = [&closeTransform] {
        closeTransform.scale = 0.5f;
    };

    clickable.onReleased = [this, &overlay, &closeTransform] {
        closeTransform.scale = 1.0f;
        toggleSettingsOverlayVisibility(overlay);
    };

    clickable.onCancelled = [&closeTransform] {
        closeTransform.scale = 1.0f;
    };

    closeButton.addComponent<Parent>(&overlay);
    auto& parentChildren = overlay.getComponent<Children>();
    parentChildren.children.push_back(&closeButton);
}

void MainMenuScene::toggleSettingsOverlayVisibility(Entity &overlay) {
    auto &sprite = overlay.getComponent<Sprite>();
    bool newVis = !sprite.visible;
    sprite.visible = newVis;

    if (overlay.hasComponent<Children>()) {
        auto&[children] = overlay.getComponent<Children>();
        for (const auto& child : children) {
            if (child && child->hasComponent<Sprite>()) {
                auto& cSprite = child->getComponent<Sprite>();
                cSprite.visible = newVis;
            }

            if (child && child->hasComponent<Collider>()) {
                auto& collider = child->getComponent<Collider>();
                collider.enabled = newVis;
            }
        }
    }
}
