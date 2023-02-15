#pragma once
#include "Asteroid.h"
#include "Grid.h"
#include "Renderer.h"
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

	Renderer& getRenderer() { return renderer; }

	// Game specific
	vector<Asteroid*>& getAsteroids();
	void addAsteroid(Asteroid* asteroid);
	void removeAsteroid(Asteroid* asteroid);

private:
	Game() : 
		isRunning(true), 
		isUpdatingActors(false),
		grid(nullptr)
	{}
	
	void processInput();
	void update(float dt);
	void render();

	Window window;
	bool isRunning;
	Renderer renderer;

	bool isUpdatingActors;
	vector<Actor*> actors;
	vector<Actor*> pendingActors;

	// Game specific
	vector<Asteroid*> asteroids;
	Grid* grid;
};