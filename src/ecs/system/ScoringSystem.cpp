//
// Created by arman on 2026-04-06.
//

#include "ScoringSystem.h"

#include "World.h"
#include "managers/BaseEvent.h"

void ScoringSystem::checkScore(World &world, const std::vector<std::unique_ptr<Entity> > &entities) {
    for (auto& entity : entities)
    {
        if (entity->hasComponent<Points>())
        {
            auto& points = entity->getComponent<Points>();
            if (points.current == points.target) {
                world.getEventManager().emit(GameStateEvent{GameState::Win});
            }
        }
    }
}
