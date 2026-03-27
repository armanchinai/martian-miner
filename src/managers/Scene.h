//
// Created by wifir on 2026-02-25.
//

#ifndef LECTURE8_SCENE_H
#define LECTURE8_SCENE_H
#include <string>
#include <SDL3/SDL_events.h>

#include "Vector2D.h"
#include "World.h"

class Scene
{
public:
    World world;

    Scene(const char* sceneName, int windowWidth, int windowHeight, const char* mapPath, const char* spriteSheetPath, std::vector<SDL_Rect> tileMappings);
    Scene(const char* sceneName, int windowWidth, int windowHeight);

    const std::string& getName() const
    {
        return name;
    }

    void update(const float deltaTime, const SDL_Event& event)
    {
        world.update(deltaTime, event);
    }

    void render()
    {
        world.render();
    }

    virtual SDL_FColor getBackgroundColour() { return {0.0f, 0.0f, 0.0f, 255.0f}; }

private:
    std::string name;
    int width;
    int height;
    void createProjectile(Vector2D position, Vector2D direction, float speed);
    SDL_FColor background{};
};

#endif //LECTURE8_SCENE_H