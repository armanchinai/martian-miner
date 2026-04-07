//
// Created by wifir on 2026-04-06.
//

#ifndef MARTIAN_MINER_MOUSEINPUTSYSTEM_H
#define MARTIAN_MINER_MOUSEINPUTSYSTEM_H
#include "SDL3/SDL_events.h"

class World;

class MouseInputSystem {
public:
    void update(World& world, const SDL_Event& event);
};

#endif //MARTIAN_MINER_MOUSEINPUTSYSTEM_H