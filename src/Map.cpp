//
// Created by qthompson on 2026-01-14.
//

#include "Map.h"

#include <cmath>
#include <iostream>
#include <utility>

#include "managers/TextureManager.h"
#include "tinyxml2.h"
#include "sstream"

Map::Map() = default;

Map::~Map() = default;

void Map::load(const char* path, SDL_Texture *spriteSheet, std::vector<SDL_Rect> mappings)
{
    this->tileTextures = spriteSheet;
    this->tileMappings = std::move(mappings);
    tinyxml2::XMLDocument document;
    document.LoadFile(path);

    auto* mapNode = document.FirstChildElement("map");

    if (!mapNode)
    {
        std::cout << "Error loading map data from '" << path << "'" << std::endl;
        return;
    }

    width = mapNode->IntAttribute("width");
    height = mapNode->IntAttribute("height");

    auto* layer = mapNode->FirstChildElement("layer");
    auto* data = layer->FirstChildElement("data");

    std::string csv = data->GetText();
    std::stringstream ss(csv);

    tileData = std::vector(height, std::vector<int>(width));
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            std::string value;
            if (!std::getline(ss, value, ','))
            {
                break;
            }
            tileData[row][col] = std::stoi(value);
        }
    }

    for (
        auto* nextGroup = layer->NextSiblingElement("objectgroup");
        nextGroup != nullptr;
        nextGroup = nextGroup->NextSiblingElement("objectgroup")
    )
    {
        const char* name = nextGroup->Attribute("name");
        if (!name) continue;
        if (std::strcmp(name, "ColliderLayer") == 0)
        {
            for (
            auto* object = nextGroup->FirstChildElement("object");
            object != nullptr;
            object = object->NextSiblingElement("object")
            )
            {
                Collider c;
                c.rect.x = object->FloatAttribute("x");
                c.rect.y = object->FloatAttribute("y");
                c.rect.w = object->FloatAttribute("width");
                c.rect.h = object->FloatAttribute("height");
                colliders.push_back(c);
            }
        }
        else if (std::strcmp(name, "ItemLayer") == 0)
        {
            for (
                auto* object = nextGroup->FirstChildElement("object");
                object != nullptr;
                object = object->NextSiblingElement("object")
                )
            {
                Vector2D pos = Vector2D(object->FloatAttribute("x"), object->FloatAttribute("y"));
                itemLocations.push_back(pos);
            }
        }
    }
}

void Map::draw(const Camera& camera) const
{
    SDL_FRect src{}, dst{};
    dst.w = dst.h = 32;

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            const int type = tileData[row][col];

            const float worldX = static_cast<float>(col) * dst.w;
            const float worldY = static_cast<float>(row) * dst.h;

            dst.x = std::round(worldX - camera.view.x);
            dst.y = std::round(worldY - camera.view.y);

            if (this->tileMappings.size() <= type)
            {
                auto mapping = this->tileMappings[type - 1];
                src.x = mapping.x;
                src.y = mapping.y;
                src.w = mapping.w;
                src.h = mapping.h;
            }

            TextureManager::draw(tileTextures, src, dst);
        }
    }
}
