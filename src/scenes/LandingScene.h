//
// Created by qthompson on 2026-03-24.
//

#ifndef MARTIAN_MINER_LANDINGSCENE_H
#define MARTIAN_MINER_LANDINGSCENE_H
#include "managers/Scene.h"


class LandingScene : public Scene
{
public:
    LandingScene(const char* name, int windowWidth, int windowHeight);
    SDL_FColor getBackgroundColour() override;
private:
    Entity* cameraEntity = nullptr;
    Entity* playerEntity = nullptr;
};


#endif //MARTIAN_MINER_LANDINGSCENE_H