//
// Created by wifir on 2026-02-04.
//

#ifndef LECTURE5_EVENTMANAGER_H
#define LECTURE5_EVENTMANAGER_H
#include <functional>

#include "BaseEvent.h"
#include "../ecs/Entity.h"

class EventManager
{
public:
    using Handler = std::function<void(const BaseEvent&)>;

    /* Emits an event to all registered listeners. */
    void emit(const BaseEvent& event) const
    {
        for (const auto& listener : listeners)
        {
            listener(event);
        }
    }

    /* Subscribes a new listener callback to receive emitted events. */
    void subscribe(const Handler& callback)
    {
        listeners.emplace_back(callback);
    }
private:
    std::vector<Handler> listeners;
};

#endif //LECTURE5_EVENTMANAGER_H