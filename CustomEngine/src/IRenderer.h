#pragma once
#include "Vector2.h"
#include "Window.h"

class IRenderer
{
public:
	enum class Flip
	{
		None = SDL_FLIP_NONE,
		Horizontal = SDL_FLIP_HORIZONTAL,
		Vertical = SDL_FLIP_VERTICAL
	};

	enum class Type
	{
		SDL, OGL
	};

	virtual ~IRenderer() {};

	virtual bool initialize(Window& window) = 0;
	virtual void beginDraw() = 0;
	virtual void draw() = 0;
	virtual void endDraw() = 0;
	virtual void close() = 0;
	virtual IRenderer::Type type() = 0;

	virtual void drawSprite(const class Actor&, const class Texture&,
		struct Rectangle, Vector2, Flip) const = 0;
};