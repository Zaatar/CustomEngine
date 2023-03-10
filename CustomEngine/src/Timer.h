#pragma once
class Timer
{
public:
	static Timer& instance()
	{
		static Timer instance;
		return instance;
	}
	Timer(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;

	// Compute delta time as the number of milliseconds since last frame
	// Clamp it so debug won't think delta time is really high during breakpoints
	unsigned int computeDeltaTime();

	// Wait if the game runs faster than the decided FPS
	void delayTime();

private:
	Timer() : frameStart(0), lastFrame(0), frameTime(0) {}
	const static unsigned int FPS = 60;
	const static unsigned int FRAME_DELAY = 1000 / FPS;
	const unsigned int MAX_DT = 50;

	// Time in milliseconds when frame starts
	unsigned int frameStart;

	// Last frame start time in milliseconds
	unsigned int lastFrame;

	// Time it took to run the loop. Used to cap framerate
	unsigned int frameTime;
};