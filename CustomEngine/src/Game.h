#pragma once
#include "Ball.h"
#include "Renderer.h"
#include "Timer.h"
#include "Vector2.h"
#include "Window.h"

#include <algorithm>
#include <vector>

using std::for_each;
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
	void loop();
	void close();

private:
	Game() : 
		isRunning(true), 
		ballPos({ 100, 100 }), 
		genericBallVelocity({ 500, 500 }), 
		firstPaddlePos({ 50, 100 }), 
		paddleVelocity({ 0, 450 }), 
		firstPaddleDirection(0), 
		secondPaddlePos({992, 100}),
		secondPaddleDirection(0),
		wallThickness(10), 
		topWall(Rectangle()),
		bottomWall(Rectangle()),
		ballsVector()
	{}
	
	void processInput();
	void update(float dt);
	void render();

	void ballMovement(Ball ball);

	Window window;
	bool isRunning;
	Renderer renderer;

	Rectangle topWall;
	Rectangle bottomWall;
	const float wallThickness = 10;

	std::vector<Ball> ballsVector;
	int ballCount = 1;

	Vector2 ballPos;
	Vector2 genericBallVelocity;
	const float ballSize = 10;

	Vector2 firstPaddlePos;
	float firstPaddleDirection;

	Vector2 secondPaddlePos;
	float secondPaddleDirection;

	Vector2 paddleVelocity;
	const float paddleWidth = 10;
	const float paddleHeight = 64;

	float dt = 0.0f;
};