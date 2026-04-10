//
// Created by wifir on 2026-01-28.
//

#ifndef LECTURE4_KEYBOARDINPUTSYSTEM_H
#define LECTURE4_KEYBOARDINPUTSYSTEM_H
#include <memory>
#include <vector>
#include <SDL3/SDL_events.h>

#include "Component.h"
#include "Entity.h"

class World;

class KeyboardInputSystem
{
public:
    void update(World& world, const SDL_Event& event);
};

#endif //LECTURE4_KEYBOARDINPUTSYSTEM_H