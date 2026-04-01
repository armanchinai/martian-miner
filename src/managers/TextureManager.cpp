//
// Created by qthompson on 2026-01-14.
//

#include "TextureManager.h"
#include "../Game.h"

#include <iostream>
#include <ostream>

extern Game* game;

std::unordered_map<std::string, SDL_Texture*> TextureManager::textures;

SDL_Texture* TextureManager::load(const char* path)
{
    if (const auto cachedTexture = textures.find(path); cachedTexture != textures.end())
    {
        return cachedTexture->second;
    }

    SDL_Surface* tempSurface = IMG_Load(path);
    if(!tempSurface)
    {
        std::cout << "Failed to load image " << path << std::endl;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, tempSurface);
    SDL_DestroySurface(tempSurface);

    if (!texture)
    {
        std::cout << "Failed to create texture " << path << std::endl;
        return nullptr;
    }

    textures[path] = texture;

    return texture;
}

void TextureManager::draw(SDL_Texture* texture, SDL_FRect src, SDL_FRect dst) {
    draw(texture, src, dst, 0);
}

void TextureManager::draw(SDL_Texture* texture, SDL_FRect src, SDL_FRect dst, float angle)
{
    SDL_FPoint center = { dst.w / 2.0f, dst.h / 2.0f };
    SDL_RenderTextureRotated(game->renderer, texture, &src, &dst, angle, &center, SDL_FLIP_NONE);
}

void TextureManager::clean()
{
    for (auto& tex : textures)
    {
        SDL_DestroyTexture(tex.second);
        tex.second = nullptr;
    }
    textures.clear();
}
