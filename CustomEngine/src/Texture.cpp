#include <sstream>

#include "Log.h"
#include "Texture.h"

Texture::Texture() : filename(""), width(0), height(0), SDLTexture(nullptr) {}

Texture::~Texture() {}

void Texture::unload()
{
	if (SDLTexture)
	{
		SDL_DestroyTexture(SDLTexture);
	}
	else
	{
		glDeleteTextures(1, &textureID);
	}
}

bool Texture::loadOGL(RendererOGL& rendererP, const string& filenameP)
{
	filename = filenameP;
	// Load from file
	SDL_Surface* surface = IMG_Load(filename.c_str());
	if (!surface)
	{
		Log::error(LogCategory::Application, "Failed to load texture file " + filename);
		return false;
	}

	width = surface->w;
	height = surface->h;
	int format = 0;

	if (surface->format->format == SDL_PIXELFORMAT_RGB24)
	{
		format = GL_RGB;
	}
	else if (surface->format->format == SDL_PIXELFORMAT_RGBA32)
	{
		format = GL_RGBA;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
	SDL_FreeSurface(surface);

	Log::info("Loaded texture: " + filename);
	// Enable bilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

bool Texture::loadSDL(RendererSDL& renderer, const string& filenameP)
{
	filename = filenameP;
	// Load from file
	SDL_Surface* surf = IMG_Load(filename.c_str());
	if (!surf)
	{
		Log::error(LogCategory::Application, "Failed to load texture file " + filename);
		return false;
	}

	width = surf->w;
	height = surf->h;

	/*if (renderer.type() == IRenderer::Type::SDL)
	{
		RendererSDL rendererSDL = RendererSDL();
		SDLTexture = SDL_CreateTextureFromSurface(rendererSDL.toSDLRenderer(), surf);
		SDL_FreeSurface(surf);
	}*/
	// Create texture from surface
	SDLTexture = SDL_CreateTextureFromSurface(renderer.toSDLRenderer(), surf);
	SDL_FreeSurface(surf);

	if (!SDLTexture)
	{
		Log::error(LogCategory::Render, "Failed to convert surface to texture for " + filename);
		return false;
	}

	Log::info("Loaded texture " + filename);

	return true;
}

void Texture::updateInfo(int& widthOut, int& heightOut)
{
	widthOut = width;
	heightOut = height;
}

void Texture::setActive() const
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}