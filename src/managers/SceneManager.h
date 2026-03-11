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
    const char* mapPath;
    int windowWidth;
    int windowHeight;
};

class SceneManager
{
    std::unordered_map<std::string, SceneParameters> sceneParameters;
    std::string pendingScene;

    void changeScene(const std::string& sceneName)
    {
        if (const auto it = sceneParameters.find(sceneName); it != sceneParameters.end())
        {
            const auto& parameters = it->second;
            currentScene = std::make_unique<Scene>(parameters.name, parameters.mapPath, parameters.windowWidth, parameters.windowHeight);
        }
        else
        {
            std::cerr << "Scene with name '" << sceneName << "' could not be found!" << std::endl;
        }
    }
public:
    std::unique_ptr<Scene> currentScene;

    void loadScene(const char* sceneName, const char* mapPath, const int windowWidth, const int windowHeight)
    {
        sceneParameters[sceneName] = {sceneName, mapPath, windowWidth, windowHeight};
    }

    void changeSceneDeferred(const std::string sceneName)
    {
        pendingScene = sceneName;
    }

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

    void render() const
    {
        if (currentScene)
        {
            currentScene->render();
        }
    }
};

#endif //LECTURE8_SCENEMANAGER_H