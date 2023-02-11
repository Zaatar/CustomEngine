#pragma once
#include "Actor.h"
#include "Assets.h"
#include "Ball.h"
#include "Renderer.h"
#include "Timer.h"
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

private:
	Game() : 
		isRunning(true), 
		isUpdatingActors(false)
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
};