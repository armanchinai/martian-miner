//
// Created by arman on 2026-04-06.
//

#ifndef MARTIAN_MINER_SCORINGSYSTEM_H
#define MARTIAN_MINER_SCORINGSYSTEM_H
#include <iostream>
#include <memory>

#include "Component.h"
#include "Entity.h"

class ScoringSystem
{
public:
    void checkScore(const std::vector<std::unique_ptr<Entity>>& entities) {
        for (auto& entity : entities)
        {
            if (entity->hasComponent<Points>())
            {
                auto& points = entity->getComponent<Points>();
                if (points.current == points.target) {
                    std::cout << "win" << std::endl;
                }
                std::cout << points.current << std::endl;
            }
        }
    }
};

#endif //MARTIAN_MINER_SCORINGSYSTEM_H