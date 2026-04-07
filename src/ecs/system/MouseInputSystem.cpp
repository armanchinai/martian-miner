//
// Created by wifir on 2026-04-06.
//

#include "system/MouseInputSystem.h"

#include "World.h"

void MouseInputSystem::update(World& world, const SDL_Event& event) {
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP || event.type == SDL_EVENT_MOUSE_MOTION) {
        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        for (auto& e : world.getEntities()) {
            if (e->hasComponent<Clickable>() && e->hasComponent<Collider>()) {
                auto& clickable = e->getComponent<Clickable>();
                auto& collider = e->getComponent<Collider>();

                if (collider.enabled) {
                    const bool inside = (mouseX >= collider.rect.x &&
                        mouseX <= collider.rect.x + collider.rect.w &&
                        mouseY >= collider.rect.y &&
                        mouseY <= collider.rect.y + collider.rect.h);

                    if (event.type == SDL_EVENT_MOUSE_MOTION) {
                        if (!inside && clickable.pressed) {
                            world.getEventManager().emit(MouseInteractionEvent(e.get(), MouseInteractionState::Cancelled));
                        }
                    }

                    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                        if (event.button.button == SDL_BUTTON_LEFT) {
                            if (inside) {
                                clickable.pressed = true;
                                world.getEventManager().emit(MouseInteractionEvent(e.get(), MouseInteractionState::Pressed));
                            }
                        }
                    }

                    if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                        if (event.button.button == SDL_BUTTON_LEFT) {
                            if (inside) {
                                clickable.pressed = false;
                                world.getEventManager().emit(MouseInteractionEvent(e.get(), MouseInteractionState::Released));
                            }
                        }
                    }
                }
            }
        }
    }
}
