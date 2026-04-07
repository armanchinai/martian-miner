//
// Created by qthompson on 2026-04-07.
//

#ifndef MARTIAN_MINER_PRERENDERSYSTEM_H
#define MARTIAN_MINER_PRERENDERSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"
#include "managers/TextureManager.h"

struct Label;

class PreRenderSystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities)
    {
        for (const auto& e : entities)
        {
            if (e->hasComponent<Label>())
            {
                if (auto& label = e->getComponent<Label>(); label.dirty)
                {
                    std::cout << "hello";
                    TextureManager::updateLabel(label);
                }
            }
        }
    }
};

#endif //MARTIAN_MINER_PRERENDERSYSTEM_H