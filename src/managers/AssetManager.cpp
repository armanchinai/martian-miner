//
// Created by wifir on 2026-02-25.
//

#include "AssetManager.h"

#include <iostream>

#include "tinyxml2.h"

std::unordered_map<std::string, Animation> AssetManager::animations;
std::unordered_map<std::string, TTF_Font*> AssetManager::fonts;

void AssetManager::loadAnimation(const std::string& clipName, const char* path)
{
    const Animation animation = loadAnimationFromXML(path);
    animations[clipName] = animation;
}

const Animation& AssetManager::getAnimation(const std::string& clipName)
{
    return animations[clipName];
}

Animation AssetManager::loadAnimationFromXML(const char* path)
{
    Animation anim;
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    auto* root = doc.FirstChildElement("animations");

    if (!root)
    {
        std::cout << "Error loading animation data from '" << path << "'" << std::endl;
    }

    for (auto* clipElem = root->FirstChildElement(); clipElem; clipElem = clipElem->NextSiblingElement())
    {
        std::string clipName = clipElem->Name();
        AnimationClip clip;

        for (auto* frameElem = clipElem->FirstChildElement("frame"); frameElem; frameElem = frameElem->NextSiblingElement("frame"))
        {
            SDL_FRect rect;

            frameElem->QueryFloatAttribute("x", &rect.x);
            frameElem->QueryFloatAttribute("y", &rect.y);
            frameElem->QueryFloatAttribute("w", &rect.w);
            frameElem->QueryFloatAttribute("h", &rect.h);

            clip.frameIndices.push_back(rect);
        }

        anim.clips[clipName] = clip;
    }

    if (!anim.clips.empty())
    {
        anim.currentClip = anim.clips.begin()->first;
    }

    return anim;
}

void AssetManager::loadFont(const std::string& fontName, const char* path, float fontSize)
{
    fonts.emplace(fontName, TTF_OpenFont(path, fontSize));
}

TTF_Font* AssetManager::getFont(const std::string& fontName)
{
    return fonts.at(fontName);
}