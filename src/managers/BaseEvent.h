//
// Created by wifir on 2026-03-30.
//

#ifndef MARTIAN_MINER_BASEEVENT_H
#define MARTIAN_MINER_BASEEVENT_H

class Entity;

enum class EventType {
    Collision,
    PlayerAction,
};

struct BaseEvent {
    EventType type{};
    virtual ~BaseEvent() = default;
};

enum class CollisionState { Enter, Stay, Exit };

struct CollisionEvent : BaseEvent {
    Entity* entityA = nullptr;
    Entity* entityB = nullptr;
    CollisionState state{};
    CollisionEvent(Entity* entityA, Entity* entityB, const CollisionState state) : entityA(entityA), entityB(entityB), state(state) {
        type = EventType::Collision;
    }
};

enum class PlayerAction { Destruct };

struct PlayerActionEvent : BaseEvent {
    Entity* player = nullptr;
    PlayerAction action{};
    PlayerActionEvent(Entity* player, const PlayerAction action) : player(player), action(action) {
        type = EventType::PlayerAction;
    };
};

#endif //MARTIAN_MINER_BASEEVENT_H
