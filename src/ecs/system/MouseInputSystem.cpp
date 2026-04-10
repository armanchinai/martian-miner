//
// Created by wifir on 2026-04-06.
//

#include "system/MouseInputSystem.h"

#include "World.h"

/*
 * MouseInputSystem
 *
 * Responsible for handling mouse interactions with clickable entities.
 */
void MouseInputSystem::update(World& world, const SDL_Event& event) {
    // Only process relevant mouse events
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP || event.type == SDL_EVENT_MOUSE_MOTION) {
        float mouseX, mouseY;
        // Get current mouse position in window coordinates
        SDL_GetMouseState(&mouseX, &mouseY);

        // Iterate over clickable entities
        for (auto& e : world.getEntities()) {
            if (e->hasComponent<Clickable>() && e->hasComponent<Collider>()) {
                auto& clickable = e->getComponent<Clickable>();
                auto& collider = e->getComponent<Collider>();

                // Only interact with enabled colliders
                if (collider.enabled) {
                    // Check if mouse is inside collider
                    const bool inside = (mouseX >= collider.rect.x &&
                        mouseX <= collider.rect.x + collider.rect.w &&
                        mouseY >= collider.rect.y &&
                        mouseY <= collider.rect.y + collider.rect.h);

                    // Handle mouse movement
                    if (event.type == SDL_EVENT_MOUSE_MOTION) {
                        // // If mouse leaves while pressed --> cancel interaction
                        if (!inside && clickable.pressed) {
                            world.getEventManager().emit(MouseInteractionEvent(e.get(), MouseInteractionState::Cancelled));
                        }
                    }

                    // Handle mouse button down
                    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                        if (event.button.button == SDL_BUTTON_LEFT) {
                            if (inside) {
                                clickable.pressed = true;
                                world.getEventManager().emit(MouseInteractionEvent(e.get(), MouseInteractionState::Pressed));
                            }
                        }
                    }

                    // Handle mouse button up
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
