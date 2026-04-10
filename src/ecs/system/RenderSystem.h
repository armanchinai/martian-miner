//
// Created by wifir on 2026-01-21.
//

#ifndef LECTURE3_RENDERSYSTEM_H
#define LECTURE3_RENDERSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"
#include "../../managers/TextureManager.h"

/*
 * RenderSystem
 *
 * Responsible for rendering visible entities to the screen. Applies camera transformations and handles animated
 * sprites.
 */
class RenderSystem
{
public:
    void render(const std::vector<std::unique_ptr<Entity>>& entities)
    {
        // Find camera entity
        Entity* cameraEntity = nullptr;
        for (auto& entity : entities)
        {
            if (entity->hasComponent<Camera>())
            {
                cameraEntity = entity.get();
                break;
            }
        }

        // If no camera exists, nothing can be rendered correctly
        if (!cameraEntity) return;
        auto& camera = cameraEntity->getComponent<Camera>();

        // Render entities
        for (auto& entity : entities)
        {
            // Only render entities with Sprite and Transform
            if (entity->hasComponent<Sprite>() && entity->hasComponent<Transform>())
            {
                auto& t = entity->getComponent<Transform>();
                auto& sprite = entity->getComponent<Sprite>();

                // Only render world-layer objects in this pass
                if (sprite.renderLayer == RenderLayer::World) {
                    // Convert world --> screen space
                    sprite.dst.x = t.position.x - camera.view.x;
                    sprite.dst.y = t.position.y - camera.view.y;

                    // Handle animation (if present)
                    if (entity->hasComponent<Animation>())
                    {
                        auto& anim = entity->getComponent<Animation>();
                        // Set source rect based on current animation frame
                        sprite.src = anim.clips[anim.currentClip].frameIndices[anim.currentFrame];
                    }

                    // Draw sprites
                    TextureManager::draw(sprite.texture, &sprite.src, &sprite.dst, t.rotation);
                }
            }
        }
    }
};

#endif //LECTURE3_RENDERSYSTEM_H