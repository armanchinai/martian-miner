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

    Scene(const char* sceneName, const char* mapPath, int windowWidth, int windowHeight);

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
private:
    std::string name;
    void createProjectile(Vector2D position, Vector2D direction, float speed);
};

#endif //LECTURE8_SCENE_H