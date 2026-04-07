#include <iostream>
#include "Game.h"

Game* game = nullptr;

int main()
{
    const int FPS = 60;
    const int desiredFrameTime = 1000/FPS;

    Uint64 ticks = 0;

    float deltaTime = 0.0f;

    game = new Game();
    game->init("Martian Miner", 1200, 900, false);
    while (game->running())
    {
        const Uint64 currentTicks = SDL_GetTicks();
        deltaTime = (currentTicks - ticks)/1000.0f;
        ticks = currentTicks;
        game->handleEvents();
        game->update(deltaTime);
        game->render();

        if (const int actualFrameTime = SDL_GetTicks() - ticks; desiredFrameTime > actualFrameTime)
        {
            SDL_Delay(desiredFrameTime - actualFrameTime);
        }
    }

    delete game;

    return 0;
}