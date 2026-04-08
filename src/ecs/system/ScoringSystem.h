//
// Created by arman on 2026-04-06.
//

#ifndef MARTIAN_MINER_SCORINGSYSTEM_H
#define MARTIAN_MINER_SCORINGSYSTEM_H
#include <iostream>
#include <memory>

#include "Component.h"
#include "Entity.h"

class World;

class ScoringSystem
{
public:
    void checkScore(World& world, const std::vector<std::unique_ptr<Entity>>& entities);
};

#endif //MARTIAN_MINER_SCORINGSYSTEM_H