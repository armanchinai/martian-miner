//
// Created by wifir on 2026-02-04.
//

#ifndef LECTURE5_COLLISION_H
#define LECTURE5_COLLISION_H
#include <SDL3/SDL_rect.h>

#include "Component.h"

class Collision
{
public:
    static bool AABB(const SDL_FRect& rectA, const SDL_FRect& rectB);
    static bool AABB(const Collider& colA, const Collider& colB);
};

#endif //LECTURE5_COLLISION_H