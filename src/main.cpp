#include <iostream>
#include "Game.h"

Game* game = nullptr;

void vector2DFuncTest()
{
    std::cout << "Beginning functionality test for Vector2D..." << std::endl;
    Vector2D a = {3, 4};
    Vector2D b = {1, 2};
    std::cout << "a = " << a.tostring() << " b = " << b.tostring() << std::endl;
    a += b;
    std::cout << "a += b; -> a = " << a.tostring() << std::endl;
    Vector2D c = a - b;
    std::cout << "c = a - b; -> c = " << c.tostring() << std::endl;
    Vector2D d = a * 2;
    std::cout << "d = a * 2; -> d = " << d.tostring() << std::endl;
    Vector2D e = 2 * a;
    std::cout << "e = 2 * a; -> e = " << e.tostring() << std::endl;
    Vector2D f = -a;
    std::cout << "f = -a; -> f = " << f.tostring() << std::endl;
    bool same = (a == b);
    std::cout << "same = a == b; -> same = " << same << std::endl;
}

int main()
{
    const int FPS = 60;
    const int desiredFrameTime = 1000/FPS;

    Uint64 ticks = 0;

    float deltaTime = 0.0f;

    game = new Game();
    game->init("Martian Miner", 60 * 16, 40 * 16, false);
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