#pragma once
#include <SDL3/sdl.h>

#include "World.h"
#include "managers/SceneManager.h"

class Game {
public:
	SDL_Renderer* renderer = nullptr;
	static std::function<void(std::string)> onSceneChangeRequest;
	World world;

	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);

	void handleEvents(); //Checks for input and system events
	void update(float deltaTime); //Handles the game logic and updates the game state
	void render(); //Redraws the frame

	void destroy(); //Frees resources

	bool running() {
		return isRunning;
	}
private:
	int frameCount = 0;
	bool isRunning = false;

	SDL_Window* window = nullptr;
	SDL_Event event;
	SceneManager sceneManager;
};
