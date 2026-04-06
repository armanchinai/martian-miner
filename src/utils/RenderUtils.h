//
// Created by wifir on 2026-04-06.
//

#ifndef MARTIAN_MINER_RENDERUTILS_H
#define MARTIAN_MINER_RENDERUTILS_H
#include "SDL3/SDL_rect.h"

class RenderUtils {
public:
    static SDL_FRect getScaledDest(const SDL_FRect &dst, const float scale) {
        return SDL_FRect(
            dst.x + dst.w * (1-scale) / 2,
            dst.y + dst.h * (1-scale) / 2,
            dst.w * scale,
            dst.h * scale
        );
    }
};

#endif //MARTIAN_MINER_RENDERUTILS_H
