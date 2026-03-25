//
// Created by qthompson on 2026-01-14.
//

#ifndef LECTURE2_MAP_H
#define LECTURE2_MAP_H

#include <SDL3/SDL.h>
#include <vector>

#include "ecs/Component.h"

class Map
{
public:
    Map();
    ~Map();

    void load(const char* path, SDL_Texture* spriteSheet, std::vector<SDL_Rect> mappings);
    void draw(const Camera& camera) const;

    SDL_Texture *tileTextures = nullptr;
    std::vector<SDL_Rect> tileMappings;
    int width{}, height{};
    std::vector<Collider> colliders;
    std::vector<Vector2D> itemLocations;
    std::vector<std::vector<int>> tileData;
};

#endif //LECTURE2_MAP_H