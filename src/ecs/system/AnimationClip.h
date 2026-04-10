//
// Created by wifir on 2026-02-25.
//

#ifndef LECTURE7_ANIMATIONCLIP_H
#define LECTURE7_ANIMATIONCLIP_H
#include <vector>
#include <SDL3/SDL_rect.h>

/*
 * AnimationClip
 *
 * Represents a sequence of frames used to play an animation. Each frame is stored as an SDL_FRect. The order of frames
 * in `frameIndices` determines the playback order.
 */
struct AnimationClip
{
    std::vector<SDL_FRect> frameIndices;
};

#endif //LECTURE7_ANIMATIONCLIP_H