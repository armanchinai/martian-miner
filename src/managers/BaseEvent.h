//
// Created by wifir on 2026-03-30.
//

#ifndef MARTIAN_MINER_BASEEVENT_H
#define MARTIAN_MINER_BASEEVENT_H

class Entity;

enum class EventType {
    Collision,
    PlayerAction,
    MouseInteraction,
    KeyboardInteraction
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

enum class PlayerAction { Interact, Death };

struct PlayerActionEvent : BaseEvent {
    Entity* player = nullptr;
    PlayerAction action{};
    PlayerActionEvent(Entity* player, const PlayerAction action) : player(player), action(action) {
        type = EventType::PlayerAction;
    }
};

enum class KeyboardInteractionState { Pressed, Released };

struct KeyboardInteractionEvent : BaseEvent {
    SDL_Keycode key;
    KeyboardInteractionState state{};
    KeyboardInteractionEvent(const SDL_Keycode key, const KeyboardInteractionState state) : key(key), state(state) {
        type = EventType::KeyboardInteraction;
    }
};

enum class MouseInteractionState { Pressed, Released, Cancelled };

struct MouseInteractionEvent : BaseEvent {
    Entity* entity = nullptr;
    MouseInteractionState state{};
    MouseInteractionEvent(Entity* entity, const MouseInteractionState state) : entity(entity), state(state) {
        type = EventType::MouseInteraction;
    }
};

#endif //MARTIAN_MINER_BASEEVENT_H
