#pragma once
#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 1042;
const int WINDOW_HEIGHT = 768;

class Window
{
public:
	Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	bool initialize();
	void close();

	SDL_Window* getSDLWindow() const { return window; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }

private:
	SDL_Window* window;
	int width, height;
};