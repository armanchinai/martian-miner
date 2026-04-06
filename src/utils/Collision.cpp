//
// Created by wifir on 2026-02-04.
//

#include "Collision.h"
#include "Entity.h"

bool Collision::AABB(const SDL_FRect& rectA, const SDL_FRect& rectB)
{
    return rectA.x + rectA.w >= rectB.x &&
        rectB.x + rectB.w >= rectA.x &&
        rectA.y + rectA.h >= rectB.y &&
        rectB.y + rectB.h >= rectA.y;
}

bool Collision::AABB(const Collider& colA, const Collider& colB)
{
    return AABB(colA.rect, colB.rect);
}

bool Collision::getCollisionParticipants(const CollisionEvent &e, const char *entityTag, const char *otherTag, Entity *&entity, Entity *&other) {
    if (e.entityA == nullptr || e.entityB == nullptr)
    {
        return false;
    }

    if (!(e.entityA->hasComponent<Collider>() && e.entityB->hasComponent<Collider>()))
    {
        return false;
    }

    auto& colA = e.entityA->getComponent<Collider>();
    auto& colB = e.entityB->getComponent<Collider>();

    if (colA.tag == entityTag && colB.tag == otherTag)
    {
        entity = e.entityA;
        other = e.entityB;
    }
    else if (colB.tag == entityTag && colA.tag == otherTag)
    {
        entity = e.entityB;
        other = e.entityA;
    }

    return entity && other;
}
