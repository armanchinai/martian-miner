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
            if (e->hasComponent<Animation>())
            {
                auto& anim = e->getComponent<Animation>();

                std::string newClip;

                if (e->hasComponent<ForceInput>())
                {
                    auto& fi = e->getComponent<ForceInput>();
                    if (fi.inputPositional.x > 0.0f && fi.inputPositional.y < 0.0f)
                    {
                        newClip = "upAndRight";
                    }
                    else if (fi.inputPositional.x < 0.0f && fi.inputPositional.y < 0.0f)
                    {
                        newClip = "upAndLeft";
                    }
                    else if (fi.inputPositional.x > 0.0f)
                    {
                        newClip = "right";
                    }
                    else if (fi.inputPositional.x < 0.0f)
                    {
                        newClip = "left";
                    }
                    else if (fi.inputPositional.y > 0.0f)
                    {
                        newClip = "idle";
                    }
                    else if (fi.inputPositional.y < 0.0f)
                    {
                        newClip = "up";
                    }
                    else
                    {
                        newClip = "idle";
                    }
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