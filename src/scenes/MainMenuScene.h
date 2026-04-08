//
// Created by wifir on 2026-04-06.
//

#ifndef MARTIAN_MINER_MENUSCENE_H
#define MARTIAN_MINER_MENUSCENE_H
#include "../managers/Scene.h"


class MainMenuScene : public Scene {
public:
    MainMenuScene(const char* name, int windowWidth, int windowHeight);
    SDL_FColor getBackgroundColour() override;
};



#endif //MARTIAN_MINER_MENUSCENE_H
