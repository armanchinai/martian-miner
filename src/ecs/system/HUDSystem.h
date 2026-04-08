//
// Created by qthompson on 2026-04-07.
//

#ifndef MARTIAN_MINER_HUDSYSTEM_H
#define MARTIAN_MINER_HUDSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class HUDSystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities)
    {
        Entity* playerEntity = nullptr;

        for (auto& e : entities)
        {
            if (e->hasComponent<PlayerTag>())
            {
                playerEntity = e.get();
                break;
            }
        }

        if (!playerEntity)
        {
            return;
        }

        auto& playerTransform = playerEntity->getComponent<Transform>();
        for (auto& e : entities)
        {
            if (e->hasComponent<Label>())
            {

            }
        }
    }
};

#endif //MARTIAN_MINER_HUDSYSTEM_H