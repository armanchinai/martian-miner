//
// Created by wifir on 2026-01-28.
//

#ifndef LECTURE4_KEYBOARDINPUTSYSTEM_H
#define LECTURE4_KEYBOARDINPUTSYSTEM_H
#include <memory>
#include <vector>
#include <SDL3/SDL_events.h>

#include "Component.h"
#include "Entity.h"

class KeyboardInputSystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const SDL_Event& event)
    {
        for (auto& e : entities)
        {
            if (e->hasComponent<ForceInput>() && e->hasComponent<PlayerTag>())
            {
                auto& fi = e->getComponent<ForceInput>();
                if (event.type == SDL_EVENT_KEY_DOWN)
                {
                    switch (event.key.key)
                    {
                    case SDLK_W:
                        fi.inputPositional.y = -100;
                        break;
                    case SDLK_S:
                        fi.inputPositional.y = 100;
                        break;
                    case SDLK_A:
                        fi.inputPositional.x = -100;
                        break;
                    case SDLK_D:
                        fi.inputPositional.x = 100;
                        break;
                    default:
                        break;
                    }
                }
                else if (event.type == SDL_EVENT_KEY_UP)
                {
                    switch (event.key.key)
                    {
                    case SDLK_W:
                        fi.inputPositional.y = 0;
                        break;
                    case SDLK_S:
                        fi.inputPositional.y = 0;
                        break;
                    case SDLK_A:
                        fi.inputPositional.x = 0;
                        break;
                    case SDLK_D:
                        fi.inputPositional.x = 0;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
};

#endif //LECTURE4_KEYBOARDINPUTSYSTEM_H