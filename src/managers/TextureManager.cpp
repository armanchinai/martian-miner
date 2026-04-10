//
// Created by qthompson on 2026-01-14.
//

#include "TextureManager.h"
#include "../Game.h"

#include <iostream>
#include <ostream>

extern Game* game;

std::unordered_map<std::string, SDL_Texture*> TextureManager::textures;

/* Loads a texture from disk (with caching) or returns an already loaded instance. */
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

/* Draws a texture using the default rotation (0 degrees). */
void TextureManager::draw(SDL_Texture* texture, const SDL_FRect* src, const SDL_FRect* dst) {
    draw(texture, src, dst, 0);
}

/* Draws a texture to the screen with optional rotation around its center. */
void TextureManager::draw(SDL_Texture* texture, const SDL_FRect* src, const SDL_FRect* dst, float angle)
{
    const SDL_FPoint center = { dst->w / 2.0f, dst->h / 2.0f };
    SDL_RenderTextureRotated(game->renderer, texture, src, dst, angle, &center, SDL_FLIP_NONE);
}

/* Frees all cached textures and clears the texture map. */
void TextureManager::clean()
{
    for (auto& tex : textures)
    {
        SDL_DestroyTexture(tex.second);
        tex.second = nullptr;
    }
    textures.clear();
}

/* Loads a cached texture for a label, or generates it if it doesn't exist. */
void TextureManager::loadLabel(Label& label)
{
    auto it = textures.find(label.textureCacheKey);
    if (it != textures.end())
    {
        label.texture = it->second;
        return;
    }

    updateLabel(label);
}

/* Rebuilds a label's texture if it has been marked as dirty (text/format changed). */
void TextureManager::updateLabel(Label& label)
{
    if (!label.dirty)
    {
        return;
    }

    if (label.texture)
    {
        SDL_DestroyTexture(label.texture);
        label.texture = nullptr;
    }

    SDL_Surface* tempSurface = TTF_RenderText_Blended(
        label.font,
        label.text.c_str(),
        label.text.size(),
        label.colour
    );

    if (!tempSurface)
    {
        std::cout << "Failed to load surface: " << label.textureCacheKey << std::endl;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(game->renderer, tempSurface);
    SDL_DestroySurface(tempSurface);

    if (!texture)
    {
        std::cout << "Failed to load texture: " << label.textureCacheKey << std::endl;
    }

    float w, h;
    SDL_GetTextureSize(texture, &w, &h);
    label.dst.w = w;
    label.dst.h = h;

    label.texture = texture;
    textures[label.textureCacheKey] = texture;

    label.dirty = false;
}