//
// Created by wifir on 2026-02-25.
//

#ifndef LECTURE7_ANIMATIONSYSTEM_H
#define LECTURE7_ANIMATIONSYSTEM_H
#include <iostream>
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class AnimationSystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const float deltaTime)
    {
        for (auto& e : entities)
        {
            if (e->hasComponent<Animation>() && e->hasComponent<ForceInput>())
            {
                auto& anim = e->getComponent<Animation>();
                auto& fi = e->getComponent<ForceInput>();

                //state system
                std::string newClip;

                if (fi.inputPositional.x > 0.0f)
                {
                    newClip = "walk_right";
                }
                else if (fi.inputPositional.x < 0.0f)
                {
                    newClip = "walk_left";
                }
                else if (fi.inputPositional.y > 0.0f)
                {
                    newClip = "walk_down";
                }
                else if (fi.inputPositional.y < 0.0f)
                {
                    newClip = "walk_up";
                }
                else
                {
                    newClip = "idle";
                }

                if (newClip != anim.currentClip)
                {
                    anim.currentClip = newClip;
                    anim.time = 0.0f;
                    anim.currentFrame = 0;
                }

                const float animationFrameSpeed = anim.speed;
                auto clip = anim.clips[anim.currentClip];
                anim.time += deltaTime;

                if (anim.time >= animationFrameSpeed)
                {
                    anim.time -= animationFrameSpeed;
                    const std::size_t totalAnimationFrames = clip.frameIndices.size();
                    anim.currentFrame = (anim.currentFrame + 1) % totalAnimationFrames;
                }
            }
        }
    }
};

#endif //LECTURE7_ANIMATIONSYSTEM_H