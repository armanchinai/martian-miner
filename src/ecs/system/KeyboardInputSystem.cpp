//
// Created by qthompson on 2026-04-07.
//

#include "KeyboardInputSystem.h"

#include "World.h"

/*
 * KeyboardInputSystem
 *
 * Responsible for emitting KeyboardInteractionEvents based on SDL keyboard events.
 */
void KeyboardInputSystem::update(World& world, const SDL_Event& event)
{
    // Key down --> Pressed event
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        world.getEventManager().emit(KeyboardInteractionEvent(event.key.key, KeyboardInteractionState::Pressed));
    }
    // Key up --> Released event
    else if (event.type == SDL_EVENT_KEY_UP)
    {
        world.getEventManager().emit(KeyboardInteractionEvent(event.key.key, KeyboardInteractionState::Released));
    }
}
