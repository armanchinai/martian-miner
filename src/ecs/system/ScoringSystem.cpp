//
// Created by arman on 2026-04-06.
//

#include "ScoringSystem.h"

#include "World.h"
#include "managers/BaseEvent.h"

/*
 * ScoringSystem
 *
 * Responsible for checking win conditions based on entity score values. Requires a reference to World.
 */

void ScoringSystem::checkScore(World &world, const std::vector<std::unique_ptr<Entity> > &entities) {
    // Only process entities with scoring data
    for (auto& entity : entities)
    {
        if (entity->hasComponent<Points>())
        {
            auto& points = entity->getComponent<Points>();
            // // Emit GameState Win event if win condition met
            if (points.current == points.target) {
                world.getEventManager().emit(GameStateEvent{GameState::Win});
            }
        }
    }
}
