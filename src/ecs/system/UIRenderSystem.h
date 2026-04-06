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

class UIRenderSystem {
public:
    void render(const std::vector<std::unique_ptr<Entity>>& entities)
    {
        for (auto& e : entities) {
            if (e->hasComponent<Transform>() && e->hasComponent<Sprite>())
            {
                auto transform = e->getComponent<Transform>();
                auto sprite = e->getComponent<Sprite>();

                if (sprite.renderLayer == RenderLayer::UI)
                {
                    sprite.dst.x = transform.position.x;
                    sprite.dst.y = transform.position.y;

                    if (sprite.visible) {
                        const SDL_FRect scaledDest = RenderUtils::getScaledDest(sprite.dst, transform.scale);
                        TextureManager::draw(sprite.texture, sprite.src, scaledDest);
                    }
                }
            }
        }
    }
};

#endif //MARTIAN_MINER_UIRENDERSYSTEM_H
