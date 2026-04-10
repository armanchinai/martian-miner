//
// Created by wifir on 2026-01-21.
//

#ifndef LECTURE3_WORLD_H
#define LECTURE3_WORLD_H
#include <memory>
#include <vector>

#include "AccelerationSystem.h"
#include "AnimationSystem.h"
#include "CameraSystem.h"
#include "CollisionSystem.h"
#include "DestructionSystem.h"
#include "Entity.h"
#include "../managers/EventManager.h"
#include "KeyboardInputSystem.h"
#include "Map.h"
#include "MouseInputSystem.h"
#include "MovementSystem.h"
#include "PreRenderSystem.h"
#include "RenderSystem.h"
#include "ScoringSystem.h"
#include "TimerSystem.h"
#include "UIRenderSystem.h"
#include "VelocitySystem.h"
#include "managers/AudioEventQueue.h"

class World
{
    Map map;
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<std::unique_ptr<Entity>> deferredEntities;
    MovementSystem movementSystem;
    RenderSystem renderSystem;
    KeyboardInputSystem keyboardInputSystem;
    CollisionSystem collisionSystem;
    EventManager eventManager;
    AnimationSystem animationSystem;
    CameraSystem cameraSystem;
    TimerSystem timerSystem;
    DestructionSystem destructionSystem;
    AccelerationSystem accelerationSystem;
    VelocitySystem velocitySystem;
    UIRenderSystem uiRenderSystem;
    MouseInputSystem mouseInputSystem;
    ScoringSystem scoringSystem;
    PreRenderSystem preRenderSystem;
    AudioEventQueue audioEventQueue;
public:
    World() = default;

    /* Main game update loop */
    void update(const float deltaTime, const SDL_Event& event)
    {
        keyboardInputSystem.update(*this, event);
        mouseInputSystem.update(*this, event);
        accelerationSystem.update(entities, deltaTime);
        velocitySystem.update(entities, deltaTime);
        movementSystem.update(entities, deltaTime);
        collisionSystem.update(*this);
        animationSystem.update(entities, deltaTime);
        cameraSystem.update(entities);
        timerSystem.update(entities, deltaTime);
        destructionSystem.update(entities);
        scoringSystem.checkScore(*this, entities);
        audioEventQueue.process();
        preRenderSystem.update(entities);
        synchronizeEntities();
        cleanup();
    }

    /* Main game render function */
    void render()
    {
        for (auto& entity : entities)
        {
            if (entity->hasComponent<Camera>())
            {
                map.draw(entity->getComponent<Camera>());
                break;
            }
        }
        renderSystem.render(entities);
        uiRenderSystem.render(entities);
    }

    /* Spawns a new entity into the world */
    Entity& createEntity()
    {
        entities.emplace_back(std::make_unique<Entity>());
        return *entities.back();
    }

    /* Spawns a new entity into the world in the next frame */
    Entity& createDeferredEntity()
    {
        deferredEntities.emplace_back(std::make_unique<Entity>());
        return *deferredEntities.back();
    }

    /* Returns a vector containing all entities */
    std::vector<std::unique_ptr<Entity>>& getEntities()
    {
        return entities;
    }

    /* Removes all inactive entities */
    void cleanup()
    {
        std::erase_if(entities, [](const std::unique_ptr<Entity>& entity)
        {
            return !entity->isActive();
        });
    }

    /* Moves newly created entities into the main entity list. */
    void synchronizeEntities()
    {
        if (!deferredEntities.empty())
        {
            std::move(
                deferredEntities.begin(),
                deferredEntities.end(),
                std::back_inserter(entities)
            );
            deferredEntities.clear();
        }
    }

    /* Returns the EventManager */
    EventManager& getEventManager()
    {
        return eventManager;
    }

    /* Returns the map */
    Map& getMap()
    {
        return map;
    }

    /* Returns the AudioEventQueue */
    AudioEventQueue& getAudioEventQueue() {
        return audioEventQueue;
    }
};

#endif //LECTURE3_WORLD_H