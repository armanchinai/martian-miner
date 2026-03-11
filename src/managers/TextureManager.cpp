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

void TextureManager::draw(SDL_Texture* texture, SDL_FRect src, SDL_FRect dst)
{
    SDL_RenderTexture(game->renderer, texture, &src, &dst);
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
