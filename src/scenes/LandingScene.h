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

    float randomFloat();

private:
    Entity* cameraEntity = nullptr;
    Entity* playerEntity = nullptr;

    Entity* titleEntity = nullptr;
    Entity* scoreEntity = nullptr;
    Entity* overlayEntity = nullptr;
    Entity* pointsCounterEntity = nullptr;

    void createGameOverOverlay(int windowWidth, int windowHeight);
    void toggleOverlayVisibility();

    void onMouseInteraction(const MouseInteractionEvent &mouseInteraction) override;
    void onCollision(const CollisionEvent &collisionEvent) override;
    void onKeyboardInteraction(const KeyboardInteractionEvent &event) override;
    void onPlayerAction(const PlayerActionEvent &playerActionEvent) override;
    void onGameStateChanged(const GameStateEvent &gameStateEvent) override;

    void createOverlayComponents();
    void updateOverlayComponents(bool isWin);
    bool gameOver = false;
};


#endif //MARTIAN_MINER_LANDINGSCENE_H