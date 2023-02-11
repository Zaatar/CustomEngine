#include "Renderer.h"


Renderer::Renderer() :SDLRenderer(nullptr) {}

bool Renderer::initialize(Window& window)
{
	SDLRenderer = SDL_CreateRenderer(window.getSDLWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!SDLRenderer)
	{
		Log::error(LogCategory::Video, "Failed to create renderer.");
		return false;
	}
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		Log::error(LogCategory::Video, "Unable to initialize SDL_image");
		return false;
	}
	return true;
}

void Renderer::beginDraw()
{
	SDL_SetRenderDrawColor(SDLRenderer, 120, 120, 255, 255);
	SDL_RenderClear(SDLRenderer);
}

void Renderer::endDraw()
{
	SDL_RenderPresent(SDLRenderer);
}

void Renderer::drawRect(const Rectangle& rect) const
{
	SDL_SetRenderDrawColor(SDLRenderer, 255, 255, 255, 255);
	SDL_Rect SDLRect = rect.toSDLRect();
	SDL_RenderFillRect(SDLRenderer, &SDLRect);
}

void Renderer::addSprite(SpriteComponent* sprite)
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

void Renderer::removeSprite(SpriteComponent* sprite)
{
	auto iter = std::find(begin(sprites), end(sprites), sprite);
	//TODO: Add if condition to check if sprite exists before we erase it.
	sprites.erase(iter);
}

void Renderer::drawSprites()
{
	for (auto sprite : sprites)
	{
		sprite->draw(*this);
	}
}

void Renderer::drawSprite(const Actor& actor,
	const class Texture& texture, Rectangle srcRect, Vector2 origin, Flip flip) const
{
	SDL_Rect destinationRect;
	Vector2 position = actor.getPosition();
	float rotation = actor.getRotation();
	float scale = actor.getScale();
	// Scale the width/height by owner's scale
	destinationRect.w = static_cast<int>(texture.getWidth() * scale);
	destinationRect.h = static_cast<int>(texture.getHeight() * scale);
	// Center the rectangle around the position of the owner
	destinationRect.x = static_cast<int> (position.x - origin.x);
	destinationRect.y = static_cast<int> (position.y - origin.y);

	SDL_Rect* srcSDL = nullptr;
	if (srcRect != Rectangle::nullRect)
	{
		srcSDL = new SDL_Rect
		{
			Maths::round(srcRect.x),
			Maths::round(srcRect.y),
			Maths::round(srcRect.width),
			Maths::round(srcRect.height)
		};
	}

	SDL_RenderCopyEx(
		SDLRenderer,
		texture.toSDLTexture(),
		srcSDL,
		&destinationRect,
		-Maths::toDegrees(rotation),
		nullptr, // rotation point, center by default,
		static_cast<SDL_RendererFlip>(flip));

	delete srcSDL;
}

void Renderer::close()
{
	SDL_DestroyRenderer(SDLRenderer);
}