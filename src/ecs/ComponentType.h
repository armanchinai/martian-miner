//
// Created by wifir on 2026-01-21.
//

#ifndef LECTURE3_COMPONENTTYPE_H
#define LECTURE3_COMPONENTTYPE_H
#include <cstddef>

// Type alias for std::size_t
// size_t is designed to represent sizes and indices because it is unsigned and a reeeaaal chonker (read: big)
using ComponentTypeID = std::size_t;

inline ComponentTypeID getComponentTypeID()
{
    static ComponentTypeID id = 0;
    return id++;
}

// Lets us use generic types
template <typename T>
ComponentTypeID getComponentTypeID() // Creates a unique numeric id for every type passed in (getComponentTypeID<Position>() -> 0)
{
    static ComponentTypeID typeID = getComponentTypeID();
    return typeID;
}

#endif //LECTURE3_COMPONENTTYPE_H