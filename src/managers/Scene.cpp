//
// Created by wifir on 2026-02-25.
//

#include "Scene.h"

#include "AssetManager.h"

Scene::Scene(const char* sceneName, const int windowWidth, const int windowHeight, const char* mapPath, const char* spriteSheetPath, const std::vector<SDL_Rect> tileMappings)
{
    name = sceneName;
    height = windowHeight / 16;
    width = windowWidth / 16;

    world.getMap().load(mapPath, TextureManager::load(spriteSheetPath), tileMappings);
    for (const Collider& collider : world.getMap().colliders)
    {
        auto& e = world.createEntity();
        e.addComponent<Transform>(Vector2D(collider.rect.x, collider.rect.y), 0.0f, 1.0f);
        auto& c = e.addComponent<Collider>("wall");
        c.rect.x = collider.rect.x;
        c.rect.y = collider.rect.y;
        c.rect.w = collider.rect.w;
        c.rect.h = collider.rect.h;

        SDL_Texture* colTex = TextureManager::load("../assets/gizmos.png");
        SDL_FRect colTexSrc = {1, 1, 30, 30};
        SDL_FRect colTexDst = {c.rect.x, c.rect.y, c.rect.w, c.rect.h};
        //e.addComponent<Sprite>(colTex, colTexSrc, colTexDst);
    }

    auto& camera(world.createEntity());
    SDL_FRect camView;
    camView.w = windowWidth;
    camView.h = windowHeight;
    camera.addComponent<Camera>(camView, world.getMap().width * 32.0f, world.getMap().height * 32.0f);
}

Scene::Scene(const char* sceneName, const int windowWidth, const int windowHeight)
{
    name = sceneName;
    height = windowHeight;
    width = windowWidth;

    auto& camera(world.createEntity());
    SDL_FRect camView;
    camView.w = windowWidth;
    camView.h = windowHeight;
    camera.addComponent<Camera>(camView, world.getMap().width * 32.0f, world.getMap().height * 32.0f);
}

void Scene::onCollision(const CollisionEvent &collisionEvent) { }

void Scene::onPlayerAction(const PlayerActionEvent &playerActionEvent) { }

void Scene::onMouseInteraction(const MouseInteractionEvent &mouseInteractionEvent) {
    if (!mouseInteractionEvent.entity->hasComponent<Clickable>()) {
        return;
    }

    if (mouseInteractionEvent.entity->hasComponent<Collider>()) {
        if (auto& collider = mouseInteractionEvent.entity->getComponent<Collider>(); !collider.enabled) {
            return;
        }
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
            break;
        default:
            break;
    }
}

void Scene::onKeyboardInteraction(const KeyboardInteractionEvent &keyboardInteractionEvent) { }
void Scene::onGameStateChanged(const GameStateEvent &gameStateEvent) { }