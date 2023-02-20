#pragma once

#include "Actor.h"
#include "IRenderer.h"
#include "Rectangle.h"
#include "Vector2.h"
#include "Window.h"


#include <SDL2/SDL_image.h>

class RendererSDL : public IRenderer
{
public:
	RendererSDL();
	~RendererSDL();
	RendererSDL(const RendererSDL&) = delete;
	RendererSDL& operator=(const RendererSDL&) = delete;

	bool initialize(Window& window);
	void beginDraw();
	void draw();
	void endDraw();

	void drawRect(const Rectangle& rect) const;
	void addSprite(class SpriteComponent* sprite);
	void removeSprite(class SpriteComponent* sprite);
	void drawSprites();
	void drawSprite(const class Actor& actor, const class Texture& texture,
		struct Rectangle srcRect, Vector2 origin, IRenderer::Flip flip) const override;
	
	SDL_Renderer* toSDLRenderer() const { return SDLRenderer; }

	IRenderer::Type type() override;

	void close();

private:
	SDL_Renderer* SDLRenderer = nullptr;
	std::vector<SpriteComponent*> sprites;
};