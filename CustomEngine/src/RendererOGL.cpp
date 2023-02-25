#include <GL/glew.h>
#include <SDL2/SDL_image.h>

#include "Actor.h"
#include "Assets.h"
#include "Log.h"
#include "Rectangle.h"
#include "RendererOGL.h"
#include "SpriteComponent.h"
#include "Vector2.h"

RendererOGL::RendererOGL() : window(nullptr), vertexArray(nullptr), context(nullptr), shader(nullptr), viewProjection(Matrix4::createSimpleViewProjection(WINDOW_WIDTH, WINDOW_HEIGHT))
	{}

RendererOGL::~RendererOGL() {}

bool RendererOGL::initialize(Window& windowP)
{
	window = &windowP;
	
	// Set OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	context = SDL_GL_CreateContext(windowP.getSDLWindow());
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		Log::error(LogCategory::Video, "Failed to initialize GLEW.");
		return false;
	}

	// On some platforms, GLEW will emit a benign error code, so clear it
	glGetError();

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		Log::error(LogCategory::Video, "Unable to initialize SDL_Image");
		return false;
	}

	vertexArray = new VertexArray(vertices, 4, indices, 6);
	return true;
}

void RendererOGL::beginDraw()
{
	glClearColor(0.45f, 0.45f, 1.0f, 1.0f);
	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Active shader and vertex array
	shader = &Assets::getShader("Transform");
	shader->use();
	shader->setMatrix4("uViewProjection",viewProjection);
	vertexArray->setActive();
}

void RendererOGL::draw()
{
	drawSprites();
}

void RendererOGL::drawSprite(const Actor& actor, const Texture& texture,
	Rectangle srcRect, Vector2 origin, Flip flip) const 
{
	Matrix4 scaleMat = Matrix4::createScale((float)texture.getWidth(), 
		(float)texture.getHeight(), 1.0f);
	Matrix4 world = scaleMat * actor.getWorldTransform();
	Matrix4 pixelTranslation = Matrix4::createTranslation(Vector3(
		-WINDOW_WIDTH / 2 - origin.x, -WINDOW_HEIGHT / 2 - origin.y, 0.0f));
	shader->setMatrix4("uWorldTransform", world * pixelTranslation);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void RendererOGL::endDraw()
{
	SDL_GL_SwapWindow(window->getSDLWindow());
}

void RendererOGL::close()
{
	SDL_GL_DeleteContext(context);
	delete vertexArray;
}

void RendererOGL::addSprite(SpriteComponent* sprite)
{
	// Insert the sprite at the right place in function of drawOrder
	int spriteDrawOrder = sprite->getDrawOrder();
	auto iter = begin(sprites);
	for (; iter != end(sprites); ++iter)
	{
		if (spriteDrawOrder < (*iter)->getDrawOrder()) break;
	}
	sprites.insert(iter, sprite);
}

void RendererOGL::removeSprite(SpriteComponent* sprite)
{
	auto iter = std::find(begin(sprites), end(sprites), sprite);
	sprites.erase(iter);
}

void RendererOGL::drawSprites()
{
	for (auto sprite : sprites)
	{
		sprite->draw(*this);
	}
}