#include "Timer.h"

#include <algorithm>
#include <SDL2/SDL_timer.h>

unsigned int Timer::computeDeltaTime()
{
	frameStart = SDL_GetTicks();
	unsigned int dt = frameStart - lastFrame;
	lastFrame = frameStart;
	return std::min(dt, MAX_DT); // clamp delta time
}

void Timer::delayTime()
{
	frameTime = SDL_GetTicks() - frameStart;
	if (frameTime < FRAME_DELAY)
		SDL_Delay(FRAME_DELAY - frameTime);
}