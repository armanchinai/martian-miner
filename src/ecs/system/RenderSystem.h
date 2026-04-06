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

class RenderSystem
{
public:
    void render(const std::vector<std::unique_ptr<Entity>>& entities)
    {
        Entity* cameraEntity = nullptr;
        for (auto& entity : entities)
        {
            if (entity->hasComponent<Camera>())
            {
                cameraEntity = entity.get();
                break;
            }
        }

        if (!cameraEntity) return;
        auto& camera = cameraEntity->getComponent<Camera>();

        for (auto& entity : entities)
        {
            if (entity->hasComponent<Sprite>() && entity->hasComponent<Transform>())
            {
                auto& t = entity->getComponent<Transform>();
                auto& sprite = entity->getComponent<Sprite>();

                if (sprite.renderLayer == RenderLayer::World) {
                    sprite.dst.x = t.position.x - camera.view.x;
                    sprite.dst.y = t.position.y - camera.view.y;

                    if (entity->hasComponent<Animation>())
                    {
                        auto& anim = entity->getComponent<Animation>();
                        sprite.src = anim.clips[anim.currentClip].frameIndices[anim.currentFrame];
                    }

                    TextureManager::draw(sprite.texture, sprite.src, sprite.dst, t.rotation);
                }
            }
        }
    }
};

#endif //LECTURE3_RENDERSYSTEM_H