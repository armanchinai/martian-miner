//
// Created by wifir on 2026-02-25.
//

#ifndef LECTURE7_ASSETMANAGER_H
#define LECTURE7_ASSETMANAGER_H
#include <string>
#include <unordered_map>

#include "Component.h"

class AssetManager
{
    static std::unordered_map<std::string, Animation> animations;
    static std::unordered_map<std::string, TTF_Font*> fonts;
    static Animation loadAnimationFromXML(const char *path);
public:
    static void loadAnimation(const std::string& clipName, const char *path);
    static void loadFont(const std::string& fontName, const char *path, float fontSize);
    static const Animation& getAnimation(const std::string& clipName);
    static TTF_Font* getFont(const std::string& fontName);
};

#endif //LECTURE7_ASSETMANAGER_H