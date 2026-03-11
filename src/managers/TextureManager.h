//
// Created by qthompson on 2026-01-14.
//

#ifndef LECTURE2_TEXTUREMANAGER_H
#define LECTURE2_TEXTUREMANAGER_H

#include <string>
#include <unordered_map>

#include "SDL3_image/SDL_image.h"

class TextureManager
{
    static std::unordered_map<std::string, SDL_Texture*> textures;
public:
    static SDL_Texture* load(const char* path);
    static void draw(SDL_Texture* texture, SDL_FRect src, SDL_FRect dst);
    static void clean();
};

#endif //LECTURE2_TEXTUREMANAGER_H