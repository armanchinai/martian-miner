//
// Created by qthompson on 2026-04-07.
//

#include "KeyboardInputSystem.h"

#include "World.h"

void KeyboardInputSystem::update(World& world, const SDL_Event& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        world.getEventManager().emit(KeyboardInteractionEvent(event.key.key, KeyboardInteractionState::Pressed));
    }
    else if (event.type == SDL_EVENT_KEY_UP)
    {
        world.getEventManager().emit(KeyboardInteractionEvent(event.key.key, KeyboardInteractionState::Released));
    }
}
