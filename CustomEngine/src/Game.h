#pragma once
#include "Asteroid.h"
#include "Camera.h"
#include "RendererOGL.h"
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

	RendererOGL& getRendererOGL() { return rendererOGL; }
	IRenderer::Type type() { return IRenderer::Type::OGL; }

private:
	Game() : 
		isRunning(true), 
		isUpdatingActors(false),
		camera(nullptr)
	{}
	
	void processInput();
	void update(float dt);
	void render();

	Window window;
	bool isRunning;
	RendererOGL rendererOGL;

	bool isUpdatingActors;
	vector<Actor*> actors;
	vector<Actor*> pendingActors;

	Camera* camera;
};