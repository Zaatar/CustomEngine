#include "Actor.h"
#include "Assets.h"
#include "Asteroid.h"
#include "AnimSpriteComponent.h"
#include "BackgroundSpriteComponent.h"
#include "Game.h"
#include "Log.h"
#include "Ship.h"
#include "SpriteComponent.h"
#include "Timer.h"

bool Game::initialize()
{
	bool isWindowInit = window.initialize();
	bool isRendererInit = renderer.initialize(window);

	int windowWidth = window.getWidth();
	int windowHeight = window.getHeight();
	
	return isWindowInit && isRendererInit;
}

void Game::load()
{
	Assets::loadTexture(renderer,
		"C://Repository//C++//CustomEngine//CustomEngine//src//Res//Airplane.png",
		"Airplane");
	Assets::loadTexture(renderer,
		"C://Repository//C++//CustomEngine//CustomEngine//src//Res//Base.png",
		"Base");
	Assets::loadTexture(renderer,
		"C://Repository//C++//CustomEngine//CustomEngine//src//Res//Missile.png",
		"Missile");
	Assets::loadTexture(renderer,
		"C://Repository//C++//CustomEngine//CustomEngine//src//Res//Projectile.png",
		"Projectile");
	Assets::loadTexture(renderer,
		"C://Repository//C++//CustomEngine//CustomEngine//src//Res//TileBrown.png",
		"TileBrown");
	Assets::loadTexture(renderer,
		"C://Repository//C++//CustomEngine//CustomEngine//src//Res//TileBrownSelected.png",
		"TileBrownSelected");
	Assets::loadTexture(renderer,
		"C://Repository//C++//CustomEngine//CustomEngine//src//Res//TileGreen.png",
		"TileGreen");
	Assets::loadTexture(renderer,
		"C://Repository//C++//CustomEngine//CustomEngine//src//Res//TileGreenSelected.png",
		"TileGreenSelected");
	Assets::loadTexture(renderer,
		"C://Repository//C++//CustomEngine//CustomEngine//src//Res//TileGrey.png",
		"TileGrey");
	Assets::loadTexture(renderer,
		"C://Repository//C++//CustomEngine//CustomEngine//src//Res//TileGreySelected.png",
		"TileGreySelected");
	Assets::loadTexture(renderer,
		"C://Repository//C++//CustomEngine//CustomEngine//src//Res//TileRed.png",
		"TileRed");
	Assets::loadTexture(renderer,
		"C://Repository//C++//CustomEngine//CustomEngine//src//Res//TileRedSelected.png",
		"TileRedSelected");
	Assets::loadTexture(renderer,
		"C://Repository//C++//CustomEngine//CustomEngine//src//Res//TileTan.png",
		"TileTan");
	Assets::loadTexture(renderer,
		"C://Repository//C++//CustomEngine//CustomEngine//src//Res//TileTanSelected.png",
		"TileTanSelected");
	Assets::loadTexture(renderer,
		"C://Repository//C++//CustomEngine//CustomEngine//src//Res//Tower.png",
		"Tower");

	grid = new Grid();
}

void Game::loop()
{
	float dt = 0;
	while (isRunning)
	{
		dt = Timer::instance().computeDeltaTime() / 1000.0f;
		processInput();
		update(dt);
		render();
		Timer::instance().delayTime();
	}
}

void Game::unload()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!actors.empty())
	{
		delete actors.back();
	}

	// Resources
	Assets::clear();
}

void Game::close()
{
	renderer.close();
	window.close();
	SDL_Quit();
}

void Game::processInput() 
{
	SDL_PumpEvents();
	// SDL Event
	SDL_Event event;
	if (SDL_PollEvent(&event)) 
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		}
	}

	// Keyboard state
	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
	// Escape: quit game
	if (keyboardState[SDL_SCANCODE_ESCAPE])
	{
		isRunning = false;
	}

	// Actor input
	isUpdatingActors = true;
	for (auto actor : actors)
	{
		actor->processInput(keyboardState);
	}
	isUpdatingActors = false;
}

void Game::update(float dt) 
{
	// Update actors
	isUpdatingActors = true;
	for (auto actor : actors)
	{
		actor->update(dt);
	}

	isUpdatingActors = false;

	// Move pending actors to actors
	for (auto pendingActor : pendingActors)
	{
		actors.emplace_back(pendingActor);
	}
	pendingActors.clear();

	// Delete dead actors
	vector<Actor*> deadActors;
	for (auto actor : actors)
	{
		if (actor->getState() == Actor::ActorState::Dead)
		{
			deadActors.emplace_back(actor);
		}
	}

	for (auto deadActor : deadActors)
	{
		delete deadActor;
	}

	// Process mouse
	int x = 0;
	int y = 0;
	Uint32 buttons = SDL_GetMouseState(&x, &y);
	if (SDL_BUTTON(buttons) & SDL_BUTTON_LEFT)
	{
		grid->processClick(x, y);
	}
}

void Game::render() 
{
	renderer.beginDraw();
	renderer.draw();
	renderer.endDraw();
}

void Game::addActor(Actor* actor)
{
	if (isUpdatingActors)
	{
		pendingActors.emplace_back(actor);
	}
	else
	{
		actors.emplace_back(actor);
	}
}

void Game::removeActor(Actor* actor)
{
	// Erase actor from the two vectors
	auto iter = std::find(begin(pendingActors), end(pendingActors), actor);
	if (iter != end(pendingActors))
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, end(pendingActors) - 1);
		pendingActors.pop_back();
	}

	iter = std::find(begin(actors), end(actors), actor);
	if (iter != end(actors))
	{
		std::iter_swap(iter, end(actors) - 1);
		actors.pop_back();
	}
}

vector<Asteroid*>& Game::getAsteroids()
{
	return asteroids;
}

void Game::addAsteroid(Asteroid* asteroid)
{
	asteroids.emplace_back(asteroid);
}

void Game::removeAsteroid(Asteroid* asteroid)
{
	auto iter = std::find(begin(asteroids), end(asteroids), asteroid);
	if (iter != asteroids.end())
	{
		asteroids.erase(iter);
	}
}