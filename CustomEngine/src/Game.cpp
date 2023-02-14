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
		"C:\\Repository\\C++\\CustomEngine\\CustomEngine\\src\\Res\\Asteroid.png",
		"Asteroid");
	Assets::loadTexture(renderer,
		"C:\\Repository\\C++\\CustomEngine\\CustomEngine\\src\\Res\\Farback01.png",
		"Farback01");
	Assets::loadTexture(renderer,
		"C:\\Repository\\C++\\CustomEngine\\CustomEngine\\src\\Res\\Farback02.png",
		"Farback02");
	Assets::loadTexture(renderer,
		"C:\\Repository\\C++\\CustomEngine\\CustomEngine\\src\\Res\\Laser.png",
		"Laser");
	Assets::loadTexture(renderer,
		"C:\\Repository\\C++\\CustomEngine\\CustomEngine\\src\\Res\\Ship.png",
		"Ship");
	Assets::loadTexture(renderer, 
		"C:\\Repository\\C++\\CustomEngine\\CustomEngine\\src\\Res\\Ship01.png", "Ship01");
	Assets::loadTexture(renderer, 
		"C:\\Repository\\C++\\CustomEngine\\CustomEngine\\src\\Res\\Ship02.png", "Ship02");
	Assets::loadTexture(renderer, 
		"C:\\Repository\\C++\\CustomEngine\\CustomEngine\\src\\Res\\Ship03.png", "Ship03");
	Assets::loadTexture(renderer, 
		"C:\\Repository\\C++\\CustomEngine\\CustomEngine\\src\\Res\\Ship04.png", "Ship04");
	Assets::loadTexture(renderer, 
		"C:\\Repository\\C++\\CustomEngine\\CustomEngine\\src\\Res\\Stars.png", "Stars");
	

	// Animated sprite
	vector<Texture*> animTextures{
		&Assets::getTexture("Ship01"),
		&Assets::getTexture("Ship02"),
		&Assets::getTexture("Ship03"),
		&Assets::getTexture("Ship04")
	};

	// Controlled ship
	Ship* ship = new Ship();
	ship->setPosition(Vector2{ 100,300 });

	// Background
	// Create the "far back" background
	vector<Texture*> bgTexsFar{
		&Assets::getTexture("Farback01"),
		&Assets::getTexture("Farback02")
	};

	Actor* bgFar = new Actor();
	BackgroundSpriteComponent* bgSpritesFar = 
		new BackgroundSpriteComponent(bgFar, bgTexsFar);
	bgSpritesFar->setScrollSpeed(-100.0f);

	// Create the close background
	Actor* bgClose = new Actor();
	std::vector<Texture*> bgTexsClose{
		&Assets::getTexture("Stars"),
		&Assets::getTexture("Stars")
	};
	BackgroundSpriteComponent* bgSpritesClose = 
		new BackgroundSpriteComponent(bgClose, bgTexsClose, 50);
	bgSpritesClose->setScrollSpeed(-200.0f);

	const int asteroidNumber = 20;
	for (int i = 0; i < asteroidNumber; ++i)
	{
		new Asteroid();
	}
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