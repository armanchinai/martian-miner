//
// Created by wifir on 2026-04-06.
//

#ifndef MARTIAN_MINER_UIRENDERSYSTEM_H
#define MARTIAN_MINER_UIRENDERSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"
#include "RenderUtils.h"
#include "managers/TextureManager.h"

/*
 * UIRenderSystem
 *
 * Responsible for rendering UI elements (sprites and labels) directly to screen space.
 */
class UIRenderSystem {
public:
    void render(const std::vector<std::unique_ptr<Entity>>& entities)
    {
        for (auto& e : entities) {
            // Only process entities with Transform (position + scale)
            if (e->hasComponent<Transform>())
            {
                auto transform = e->getComponent<Transform>();

                // Render UI Sprites
                if (e->hasComponent<Sprite>())
                {
                    auto sprite = e->getComponent<Sprite>();

                    // Only render UI-layer sprites
                    if (sprite.renderLayer == RenderLayer::UI)
                    {
                        // Position directly in screen space
                        sprite.dst.x = transform.position.x;
                        sprite.dst.y = transform.position.y;

                        // Draw if visible
                        if (sprite.visible) {
                            // Apply scaling to destination rect
                            const SDL_FRect scaledDest = RenderUtils::getScaledDest(sprite.dst, transform.scale);
                            TextureManager::draw(sprite.texture, &sprite.src, &scaledDest);
                        }
                    }
                } else if (e->hasComponent<Label>()) // Render Labels (Text)
                {
                    auto label = e->getComponent<Label>();

                    // Position directly in screen space
                    label.dst.x = transform.position.x;
                    label.dst.y = transform.position.y;

                    // Draw if visible
                    if (label.visible)
                    {
                        // Apply scaling
                        const SDL_FRect scaledDest = RenderUtils::getScaledDest(label.dst, transform.scale);
                        TextureManager::draw(label.texture, nullptr, &scaledDest);
                    }
                }
            }
        }
    }
};

#endif //MARTIAN_MINER_UIRENDERSYSTEM_H
