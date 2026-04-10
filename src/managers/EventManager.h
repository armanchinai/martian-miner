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

    void emit(const BaseEvent& event) const
    {
        for (const auto& listener : listeners)
        {
            listener(event);
        }
    }


    void subscribe(const Handler& callback)
    {
        listeners.emplace_back(callback);
    }
private:
    std::vector<Handler> listeners;
};

#endif //LECTURE5_EVENTMANAGER_H