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
private:
    Entity& createSettingsOverlay(int windowWidth, int windowHeight);
    Entity& createCogButton(int windowWidth, int windowHeight, Entity& overlay);
    void createSettingsUIComponents(Entity& overlay);
    void toggleSettingsOverlayVisibility(Entity& overlay);
};



#endif //MARTIAN_MINER_MENUSCENE_H
