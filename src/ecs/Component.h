//
// Created by wifir on 2026-01-21.
//

#ifndef LECTURE3_COMPONENT_H
#define LECTURE3_COMPONENT_H
#include <functional>
#include <unordered_map>
#include <SDL3/SDL_render.h>

#include "AnimationClip.h"
#include "Entity.h"
#include "../utils/Vector2D.h"

struct Transform
{
    Vector2D position{};
    float rotation{};
    float scale{};
    Vector2D prevPosition{};
};

struct Acceleration {
    Vector2D direction{};
    float magnitude{};
};

struct PhysicsObject {
    float mass = 1.0f;
    float gravity = 9.87f;
    bool isGravityEnabled = true;
    float angle = 0.0f;
};

struct ForceInput {
    Vector2D inputPositional;
    float inputAngular{};
};

struct Velocity
{
    Vector2D direction{};
    float magnitude{};
};

enum class RenderLayer
{
    World,
    UI
};

struct Sprite
{
    SDL_Texture* texture = nullptr;
    SDL_FRect src{};
    SDL_FRect dst{};
    RenderLayer renderLayer = RenderLayer::World;
    bool visible = true;
};

struct Collider
{
    std::string tag;
    SDL_FRect rect{};
    bool isColliding;
    bool enabled = true;
};

struct Animation
{
    std::unordered_map<std::string, AnimationClip> clips{};
    std::string currentClip{};
    float time{};
    int currentFrame{};
    float speed = 0.1f;
};

struct Camera
{
    SDL_FRect view;
    float worldWidth;
    float worldHeight;
};

struct TimedSpawner
{
    float spawnInterval;
    std::function<void()> spawnCallback;
    float timer;
};

struct Clickable {
    std::function<void()> onPressed{};
    std::function<void()> onReleased{};
    std::function<void()> onCancelled{};
    bool pressed = false;
};

struct Parent {
    Entity* parent = nullptr;
};

struct Children {
    std::vector<Entity*> children{};
};

struct SceneState
{
    int coinsCollected = 0;
};

struct PlayerTag
{
    bool withinLandingZone = false;
};
struct ProjectileTag{};

#endif //LECTURE3_COMPONENT_H