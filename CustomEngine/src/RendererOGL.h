#pragma once

#include <vector>

#include "IRenderer.h"
#include "Shader.h"
#include "Vector2.h"
#include "VertexArray.h"


class RendererOGL : public IRenderer
{
public:
	RendererOGL();
	virtual ~RendererOGL();
	RendererOGL(const RendererOGL&) = delete;
	RendererOGL& operator=(const RendererOGL&) = delete;

	Shader* shader;

	bool initialize(Window& window);
	void beginDraw();
	void draw();
	void endDraw();

	void addSprite(class SpriteComponent* sprite);
	void removeSprite(class SpriteComponent* sprite);
	void drawSprite(const class Actor& actor, const class Texture& texture,
		Rectangle srcRect, Vector2 origin, Flip flip) const;

	void addMesh(class MeshComponent* mesh);
	void removeMesh(class MeshComponent* mesh);
	void setViewMatrix(const Matrix4& viewP);

	void close();
	IRenderer::Type type() { return Type::OGL; }

private:
	void drawSprites();
	void drawMeshes();

	Window* window;
	VertexArray* spriteVertexArray;
	SDL_GLContext context;
	Matrix4 spriteViewProjection;
	Matrix4 view;
	Matrix4 projection;

	std::vector<class SpriteComponent*> sprites;
	std::vector<class MeshComponent*> meshes;
};