#pragma once
#include "Asteroid.h"
#include "Grid.h"
#include "RendererSDL.h"
#include "Vector2.h"
#include "Window.h"

#include <vector>

using std::vector;

class Game
{
public:
	static Game& instance()
	{
		static Game inst;
		return inst;
	}

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;

	bool initialize();
	void load();
	void loop();
	void unload();
	void close();

	void addActor(Actor* actor);
	void removeActor(Actor* actor);

	RendererSDL& getRenderer() { return renderer; }

	// Game specific
	vector<Asteroid*>& getAsteroids();
	void addAsteroid(Asteroid* asteroid);
	void removeAsteroid(Asteroid* asteroid);
	class Grid& getGrid() { return *grid; }
	std::vector<class Enemy*>& getEnemies() { return enemies; }

private:
	Game() : 
		isRunning(true), 
		isUpdatingActors(false),
		grid(nullptr),
		nextEnemyTimer(0.0f)
	{}
	
	void processInput();
	void update(float dt);
	void render();

	Window window;
	bool isRunning;
	RendererSDL renderer;

	bool isUpdatingActors;
	vector<Actor*> actors;
	vector<Actor*> pendingActors;

	// Game specific
	vector<Asteroid*> asteroids;
	Grid* grid;
	std::vector<class Enemy*> enemies;
	float nextEnemyTimer;
};