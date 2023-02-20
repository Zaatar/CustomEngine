#include <GL/glew.h>

#include "Log.h"
#include "Window.h"

Window::Window() : window(nullptr), width(WINDOW_WIDTH), height(WINDOW_HEIGHT) {}

bool Window::initialize()
{
	int sdlInitResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlInitResult != 0)
	{
		Log::error(LogCategory::Video, "Unable to initialize SDL");
		return false;
	}
	//window = SDL_CreateWindow("Tower Defense", 100, 100, width, height, 0); // 0 is a flag we will use later
	window = SDL_CreateWindow("Parallaxes", 100, 100, width, height, SDL_WINDOW_OPENGL);
	if (!window)
	{
		Log::error(LogCategory::System, "Failed to create window");
		return false;
	}
	return true;
}

void Window::close()
{
	SDL_DestroyWindow(window);
}