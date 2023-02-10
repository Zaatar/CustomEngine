#pragma once

#include "Log.h"
#include "Rectangle.h"
#include "Window.h"


#include <SDL2/SDL_image.h>

class Renderer
{
public:
	Renderer();
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	bool initialize(Window& window);
	void beginDraw();
	void drawRect(Rectangle& rect);
	void endDraw();
	void close();

	inline SDL_Renderer* toSDLRenderer() const { return SDLRenderer; }

private:
	SDL_Renderer* SDLRenderer = nullptr;
};