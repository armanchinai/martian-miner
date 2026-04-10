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

/*
 * AnimationSystem
 *
 * Updates animation state and frame progression for entities.
 */
class AnimationSystem
{
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const float deltaTime)
    {
        for (auto& e : entities)
        {
            // Only process entities with an Animation component
            if (e->hasComponent<Animation>())
            {
                auto& anim = e->getComponent<Animation>();

                std::string newClip;

                // Set animation clip
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
                    // Default to idle if no input component exists
                    newClip = "idle";
                }

                // Handle clip switching
                if (newClip != anim.currentClip)
                {
                    anim.currentClip = newClip;
                    anim.time = 0.0f;
                    anim.currentFrame = 0;
                }

                // Progress animation timing
                const float animationFrameSpeed = anim.speed;
                // Get active clip
                auto clip = anim.clips[anim.currentClip];
                // Accumulate elapsed time
                anim.time += deltaTime;

                // Advance to next frame if enough time elapsed
                if ((anim.time >= animationFrameSpeed && anim.looping) ||
                    (anim.time >= animationFrameSpeed && anim.currentFrame < clip.frameIndices.size() - 1))
                {
                    // Recalibrate accumulated time
                    anim.time -= animationFrameSpeed;
                    const std::size_t totalAnimationFrames = clip.frameIndices.size();

                    // Advance frame (wrap if looping)
                    anim.currentFrame = (anim.currentFrame + 1) % totalAnimationFrames;
                }
            }
        }
    }
};

#endif //LECTURE7_ANIMATIONSYSTEM_H