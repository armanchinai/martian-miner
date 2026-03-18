//
// Created by qthompson on 2026-01-07.
//

#include "Game.h"
#include "Map.h"

#include <iostream>
#include <ostream>

#include "managers/AssetManager.h"

std::function<void(std::string)> Game::onSceneChangeRequest;

Game::Game()
{

}

Game::~Game()
{
    destroy();
}

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

        isRunning = true;
    }
    else
    {
        isRunning = false;
    }

    // Load Assets
    AssetManager::loadAnimation("player", "../assets/animations/lander_animations.xml");
    AssetManager::loadAnimation("enemy", "../assets/animations/bird_animations.xml");

    // Load Scenes
    sceneManager.loadScene("level1", "../assets/map1.tmx", width, height);
    sceneManager.loadScene("level2", "../assets/map2.tmx", width, height);
    sceneManager.changeSceneDeferred("level1");

    onSceneChangeRequest = [this](const std::string& sceneName)
    {
        std::cout << sceneName << ":" << sceneManager.currentScene->getName() << std::endl;
        if (sceneManager.currentScene->getName() == "level2" && sceneName == "level2")
        {
            std::cout << "You win!" << std::endl;
            isRunning = false;
            return;
        }

        if (sceneName == "gameover")
        {
            std::cout << "You lose!" << std::endl;
            isRunning = false;
            return;
        }

        sceneManager.changeSceneDeferred(sceneName);
    };
}

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
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
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