//
// Created by wifir on 2026-01-21.
//

#ifndef LECTURE3_ENTITY_H
#define LECTURE3_ENTITY_H

#include <array>
#include <bitset>
#include <cstddef>

#include "ComponentType.h"
constexpr std::size_t MAX_COMPONENTS = 32;

using ComponentBitSet = std::bitset<MAX_COMPONENTS>;

using ComponentArray = std::array<void*, MAX_COMPONENTS>;

class Entity
{
    bool active = true;
    ComponentArray componentArray{};
    ComponentBitSet componentBitSet{};
public:
    bool isActive()
    {
        return active;
    }
    void destroy()
    {
        active = false;
    }

    template <typename T>
    bool hasComponent()
    {
        return componentBitSet[getComponentTypeID<T>()];
    }
    template <typename T, typename... mArgs>
    T& addComponent(mArgs&&...args)
    {
        T* component(new T{std::forward<mArgs>(args)...});
        componentArray[getComponentTypeID<T>()] = component;
        componentBitSet[getComponentTypeID<T>()] = true;

        return *component;
    }
    template <typename T>
    T& getComponent()
    {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }
    template <typename T>
    void deactivateComponent()
    {
        componentBitSet[getComponentTypeID<T>()] = false;
    }
};

#endif //LECTURE3_ENTITY_H
