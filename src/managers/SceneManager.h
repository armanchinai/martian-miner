//
// Created by wifir on 2026-02-25.
//

#ifndef LECTURE8_SCENEMANAGER_H
#define LECTURE8_SCENEMANAGER_H
#include <memory>
#include <string>
#include <unordered_map>

#include "Scene.h"

struct SceneParameters
{
    const char* name;
    std::function<std::unique_ptr<Scene>()> factory;
    int windowWidth;
    int windowHeight;
};

class SceneManager
{
    std::unordered_map<std::string, SceneParameters> sceneParameters;
    std::string pendingScene;

    /* Changes the current scene using a registered scene factory, if it exists. */
    void changeScene(const std::string& sceneName)
    {
        if (const auto it = sceneParameters.find(sceneName); it != sceneParameters.end())
        {
            const auto& parameters = it->second;
            currentScene = parameters.factory();
        }
        else
        {
            std::cerr << "Scene with name '" << sceneName << "' could not be found!\n";
        }
    }
public:
    std::unique_ptr<Scene> currentScene;

    template <typename SceneType>

    /* Registers a scene with a factory function and injects event handlers for runtime behavior. */
    void loadScene(const char* sceneName, int windowWidth, int windowHeight)
    {
        sceneParameters[sceneName] = {
            sceneName,
            [=]() {
                auto scene = std::make_unique<SceneType>(sceneName, windowWidth, windowHeight);

                // Inject SceneSwap Handler
                Scene& base = *scene;
                base.world.getEventManager().subscribe(
                    [this](const BaseEvent& e) {
                        if (e.type != EventType::SceneSwap) {
                            return;
                        }

                        const auto& sse = static_cast<const SceneSwapEvent&>(e);
                        changeSceneDeferred(sse.nextSceneName);
                    }
                );

                // Inject On(event) handlers
                base.world.getEventManager().subscribe([&] (const BaseEvent& e) {
                    if (e.type == EventType::Collision) {
                        base.onCollision(dynamic_cast<const CollisionEvent &>(e));
                    } else if (e.type == EventType::MouseInteraction) {
                        base.onMouseInteraction(dynamic_cast<const MouseInteractionEvent &>(e));
                    } else if (e.type == EventType::KeyboardInteraction) {
                        base.onKeyboardInteraction(dynamic_cast<const KeyboardInteractionEvent &>(e));
                    } else if (e.type == EventType::GameState) {
                        base.onGameStateChanged(dynamic_cast<const GameStateEvent &>(e));
                    } else if (e.type == EventType::PlayerAction) {
                        base.onPlayerAction(dynamic_cast<const PlayerActionEvent &>(e));
                    }
                });

                return scene;
            },
            windowWidth,
            windowHeight
        };
    }

    /* Returns the current scene's background colour. */
    SDL_FColor getBackgroundColour() const
    {
        return currentScene->getBackgroundColour();
    }

    /* Queues a scene change to be executed at a safe point in the update loop. */
    void changeSceneDeferred(const std::string sceneName)
    {
        pendingScene = sceneName;
    }

    /* Updates the current scene and performs any pending scene transitions. */
    void update(const float deltaTime, const SDL_Event& event)
    {
        if (currentScene)
        {
            currentScene->update(deltaTime, event);
        }
        if (!pendingScene.empty())
        {
            changeScene(pendingScene);
            pendingScene.clear();
        }
    }

    /* Renders the currently active scene. */
    void render() const
    {
        if (currentScene)
        {
            currentScene->render();
        }
    }
};

#endif //LECTURE8_SCENEMANAGER_H