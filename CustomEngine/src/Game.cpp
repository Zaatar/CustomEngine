#include "Game.h"

bool Game::initialize()
{
	bool isWindowInit = window.initialize();
	bool isRendererInit = renderer.initialize(window);

	int windowWidth = window.getWidth();
	int windowHeight = window.getHeight();
	topWall = { 0, 0, static_cast<float>(windowWidth), wallThickness };
	bottomWall = { 0, windowHeight - wallThickness, static_cast<float>(windowWidth), wallThickness };


	for (int i = 0; i < ballCount; ++i)
	{
		Vector2 ballPosition = { ballPos.x - 50 * i, ballPos.y - 50 * i };
		Vector2 ballVelocity = { genericBallVelocity.x - 50 * i, genericBallVelocity.y - 50 * i };
		ballsVector.push_back(Ball(ballPosition, ballVelocity));
	}
	
	return isWindowInit && isRendererInit;
}

void Game::loop()
{
	Timer timer;
	dt = 0;
	while (isRunning)
	{
		dt = timer.computeDeltaTime() / 1000.0f;
		processInput();
		update(dt);
		render();
		timer.delayTime();
	}
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
	// First Paddle move
	if (keyboardState[SDL_SCANCODE_W])
	{
		firstPaddleDirection = -1;
	}
	if (keyboardState[SDL_SCANCODE_S])
	{
		firstPaddleDirection = 1;
	}

	// Second Paddle Move
	if (keyboardState[SDL_SCANCODE_UP])
	{
		secondPaddleDirection = -1;
	}
	if (keyboardState[SDL_SCANCODE_DOWN])
	{
		secondPaddleDirection = 1;
	}
}

void Game::update(float dt) 
{
	// Paddle move
	firstPaddlePos += paddleVelocity * dt * firstPaddleDirection;
	if (firstPaddlePos.y < paddleHeight / 2 + wallThickness)
	{
		firstPaddlePos.y = paddleHeight / 2 + wallThickness;
	}
	if (firstPaddlePos.y > window.getHeight() - paddleHeight / 2 - wallThickness)
	{
		firstPaddlePos.y = window.getHeight() - paddleHeight / 2 - wallThickness;
	}

	secondPaddlePos += paddleVelocity * dt * secondPaddleDirection;
	if (secondPaddlePos.y < paddleHeight / 2 + wallThickness)
	{
		secondPaddlePos.y = paddleHeight / 2 + wallThickness;
	}
	if (secondPaddlePos.y > window.getHeight() - paddleHeight / 2 - wallThickness)
	{
		secondPaddlePos.y = window.getHeight() - paddleHeight / 2 - wallThickness;
	}

	// Ball move
	for (int i = 0; i < ballsVector.size(); ++i)
	{
		ballsVector[i].vectorPosition += ballsVector[i].vectorVelocity * dt;
		if (ballsVector[i].vectorPosition.y < ballSize / 2 + wallThickness)
		{
			ballsVector[i].vectorPosition.y = ballSize / 2 + wallThickness;
			ballsVector[i].vectorVelocity.y *= -1;
		}
		else if (ballsVector[i].vectorPosition.y > window.getHeight() - ballSize / 2 - wallThickness)
		{
			ballsVector[i].vectorPosition.y = window.getHeight() - ballSize / 2 - wallThickness;
			ballsVector[i].vectorVelocity.y *= -1;
		}

		//Ball - Paddle Collision
		Vector2 firstPaddleDiff = ballsVector[i].vectorPosition - firstPaddlePos;
		if (fabsf(firstPaddleDiff.y) <= paddleHeight / 2
			&& fabsf(firstPaddleDiff.x <= paddleWidth / 2 + ballSize / 2)
			&& ballsVector[i].vectorVelocity.x < 0)
		{
			ballsVector[i].vectorVelocity.x *= -1;
			ballsVector[i].vectorPosition.x = firstPaddlePos.x + paddleWidth / 2 + ballSize / 2;
		}

		Vector2 secondPaddleDiff = ballsVector[i].vectorPosition - secondPaddlePos;
		if (fabsf(secondPaddleDiff.y) <= paddleHeight / 2
			&& fabsf(secondPaddleDiff.x >= paddleWidth / 2 + ballSize / 2)
			&& ballsVector[i].vectorVelocity.x > 1)
		{
			ballsVector[i].vectorVelocity.x *= -1;
			ballsVector[i].vectorPosition.x = secondPaddlePos.x - paddleWidth / 2 - ballSize / 2;
		}

		//Restart automatically
		if (ballsVector[i].vectorPosition.x < 0 || ballsVector[i].vectorPosition.x > window.getWidth())
		{
			ballsVector[i].vectorPosition.x *= -1;
			ballsVector[i].vectorPosition.x = window.getWidth() / 2.0f;
		}
	}
}

void Game::render() 
{
	renderer.beginDraw();
	
	renderer.drawRect(topWall);
	renderer.drawRect(bottomWall);

	for (int i = 0; i < ballsVector.size(); ++i)
	{
		Rectangle ballRect = { ballsVector[i].vectorPosition.x - ballSize / 2, ballsVector[i].vectorPosition.y - ballSize / 2, ballSize, ballSize };
		renderer.drawRect(ballRect);
	}

	Rectangle firstPaddleRect = { firstPaddlePos.x - paddleWidth / 2, firstPaddlePos.y - paddleHeight / 2, paddleWidth, paddleHeight };
	renderer.drawRect(firstPaddleRect);

	Rectangle secondPaddleRect = { secondPaddlePos.x - paddleWidth / 2, secondPaddlePos.y - paddleHeight / 2, paddleWidth, paddleHeight };
	renderer.drawRect(secondPaddleRect);

	renderer.endDraw();
}