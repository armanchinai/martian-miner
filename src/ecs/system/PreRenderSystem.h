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

/*
 * PreRenderSystem
 *
 * Responsible for preparing renderable assets before the actual render step.
 */
class PreRenderSystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities)
    {
        for (const auto& e : entities)
        {
            // Only process entities with a Label component
            if (e->hasComponent<Label>())
            {
                // Only perform update on dirty labels.
                if (auto& label = e->getComponent<Label>(); label.dirty)
                {
                    TextureManager::updateLabel(label);
                }
            }
        }
    }
};

#endif //MARTIAN_MINER_PRERENDERSYSTEM_H