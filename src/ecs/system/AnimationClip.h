//
// Created by wifir on 2026-02-25.
//

#ifndef LECTURE7_ANIMATIONCLIP_H
#define LECTURE7_ANIMATIONCLIP_H
#include <vector>
#include <SDL3/SDL_rect.h>

struct AnimationClip
{
    std::vector<SDL_FRect> frameIndices;
};

#endif //LECTURE7_ANIMATIONCLIP_H