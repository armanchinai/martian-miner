//
// Created by wifir on 2026-02-25.
//

#include "AssetManager.h"

#include <iostream>

#include "tinyxml2.h"

std::unordered_map<std::string, Animation> AssetManager::animations;
std::unordered_map<std::string, TTF_Font*> AssetManager::fonts;

/* Loads an animation from an XML file and stores it under the given name. */
void AssetManager::loadAnimation(const std::string& clipName, const char* path)
{
    const Animation animation = loadAnimationFromXML(path);
    animations[clipName] = animation;
}

/* Returns a reference to the animation stored under the given name. */
const Animation& AssetManager::getAnimation(const std::string& clipName)
{
    return animations[clipName];
}

/* Loads an Animation object from an XML file and parses its clips and frames. */
Animation AssetManager::loadAnimationFromXML(const char* path)
{
    Animation anim;

    // Load XML file from memory
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    // Get root <animations> element
    auto* root = doc.FirstChildElement("animations");

    if (!root)
    {
        std::cout << "Error loading animation data from '" << path << "'" << std::endl;
    }

    // Iterate through each animation clip node (e.g. <walk>, <run>)
    for (auto* clipElem = root->FirstChildElement(); clipElem; clipElem = clipElem->NextSiblingElement())
    {
        // Use XML tag name as clip name
        std::string clipName = clipElem->Name();
        AnimationClip clip;

        // Iterate through each <frame> inside the clip
        for (auto* frameElem = clipElem->FirstChildElement("frame"); frameElem; frameElem = frameElem->NextSiblingElement("frame"))
        {
            // Stores frame rectangle (x, y, w, h)
            SDL_FRect rect;

            // Read frame attributes from XML
            frameElem->QueryFloatAttribute("x", &rect.x);
            frameElem->QueryFloatAttribute("y", &rect.y);
            frameElem->QueryFloatAttribute("w", &rect.w);
            frameElem->QueryFloatAttribute("h", &rect.h);

            // Add frame to clip sequence
            clip.frameIndices.push_back(rect);
        }

        // Store completed clip in animation map
        anim.clips[clipName] = clip;
    }

    // Set default clip to first available clip (if any exist)
    if (!anim.clips.empty())
    {
        anim.currentClip = anim.clips.begin()->first;
    }

    return anim;
}

/* Loads a font from file and stores it under the given name. */
void AssetManager::loadFont(const std::string& fontName, const char* path, float fontSize)
{
    fonts.emplace(fontName, TTF_OpenFont(path, fontSize));
}

/* Retrieves a font by name from the asset manager. */
TTF_Font* AssetManager::getFont(const std::string& fontName)
{
    return fonts.at(fontName);
}