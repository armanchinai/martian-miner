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
    static Animation loadAnimationFromXML(const char *path);
public:
    static void loadAnimation(const std::string& clipName, const char *path);
    static const Animation& getAnimation(const std::string& clipName);
};

#endif //LECTURE7_ASSETMANAGER_H