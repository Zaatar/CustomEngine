#pragma once

#include <vector>

#include "IRenderer.h"
#include "Vector2.h"
#include "VertexArray.h"


class RendererOGL : public IRenderer
{
public:
	RendererOGL();
	virtual ~RendererOGL();
	RendererOGL(const RendererOGL&) = delete;
	RendererOGL& operator=(const RendererOGL&) = delete;

	bool initialize(Window& window);
	void beginDraw();
	void draw();
	void endDraw();

	void addSprite(class SpriteComponent* sprite);
	void removeSprite(class SpriteComponent* sprite);
	void drawSprite(const class Actor& actor, const class Texture& texture,
		Rectangle srcRect, Vector2 origin, Flip flip) const;

	void close();
	IRenderer::Type type() { return Type::OGL; }

private:
	void drawSprites();

	Window* window;
	VertexArray* vertexArray;
	SDL_GLContext context;
	std::vector<class SpriteComponent*> sprites;
};