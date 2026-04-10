//
// Created by qthompson on 2026-01-07.
//

#include "Game.h"
#include "Map.h"

#include <iostream>
#include <ostream>

#include "managers/AssetManager.h"
#include "scenes/LandingScene.h"
#include "scenes/MainMenuScene.h"

Game::Game()
{

}

Game::~Game()
{
    destroy();
}

/* Initializes the game */
void Game::init(const char* title, const int width, const int height, const bool fullscreen)
{
    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_InitSubSystem(SDL_INIT_VIDEO))
    {
        std::cout << "Subsystem initialized..." << std::endl;
        window = SDL_CreateWindow(title, width, height, flags);
        if (window)
        {
            std::cout << "Window created..." << std::endl;
        }

        renderer = SDL_CreateRenderer(window, nullptr);

        if (renderer)
        {
            std::cout << "Renderer created..." << std::endl;
        }
        else
        {
            std::cout << "Renderer could not be created" << std::endl;
            return;
        }

        if (TTF_Init() != 1)
        {
            std::cout << "SDL_TTF Init failed!" << std::endl;
        }

        isRunning = true;
    }
    else
    {
        isRunning = false;
    }

    // Load Audio
    audioManager.loadAudio("thruster", "../assets/audio/fronbondi_skegs-sfx-looking-straight-into-a-burning-rocket-engine-sound-effect-283448.ogg");
    audioManager.loadAudio("explosion", "../assets/audio/482993__v-ktor__large-explosion-1.ogg");
    audioManager.loadAudio("coin", "../assets/audio/driken5482-retro-coin-4-236671.ogg");
    audioManager.loadAudio("button", "../assets/audio/freesound_community-menu-button-88360.ogg");

    // Load Scenes
    sceneManager.loadScene<LandingScene>("game", width, height);
    sceneManager.loadScene<MainMenuScene>("menu", width, height);
    sceneManager.changeSceneDeferred("menu");
}

/* Handles SDL events */
void Game::handleEvents()
{
    SDL_PollEvent(&event);

    switch (event.type)
    {
        case SDL_EVENT_QUIT:
        isRunning = false;
        break;
        default:
        break;
    }
}

void Game::update(const float deltaTime)
{
    sceneManager.update(deltaTime, event);
}

void Game::render()
{
    auto [r, g, b, a] = sceneManager.getBackgroundColour();
    SDL_SetRenderDrawColorFloat(renderer, r, g, b, a);
    SDL_RenderClear(renderer);

    sceneManager.render();

    SDL_RenderPresent(renderer); // For page swapping
}

void Game::destroy()
{
    TextureManager::clean();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Terminated." << std::endl;
}