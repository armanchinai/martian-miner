//
// Created by wifir on 2026-02-04.
//

#ifndef LECTURE5_COLLISION_H
#define LECTURE5_COLLISION_H
#include <SDL3/SDL_rect.h>

#include "Component.h"
#include "managers/BaseEvent.h"

class Entity;

class Collision
{
public:
    static bool AABB(const SDL_FRect& rectA, const SDL_FRect& rectB);
    static bool AABB(const Collider& colA, const Collider& colB);
    static bool getCollisionParticipants(const CollisionEvent& e, const char* entityTag, const char* otherTag, Entity*& entity, Entity*& other);
};

#endif //LECTURE5_COLLISION_H