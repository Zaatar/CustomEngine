#include "Actor.h"
#include "Game.h"
#include "Log.h"
#include "Timer.h"
#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(Actor* ownerP, Texture& textureP, int drawOrderP) :
	Component(ownerP),
	texture(textureP),
	drawOrder(drawOrderP),
	texWidth(textureP.getWidth()),
	texHeight(textureP.getHeight()),
	delayDraw(false)
{
	owner.getGame().getRendererOGL().addSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	owner.getGame().getRendererOGL().removeSprite(this);
}

void SpriteComponent::setTexture(const Texture& textureP)
{
	texture = textureP;
	texture.updateInfo(texWidth, texHeight);
}

void SpriteComponent::draw(IRenderer& renderer)
{
	if (delayDraw)
	{
		delayCounter += Timer::instance().computeDeltaTime() / 1000.0f;
		while (delayCounter < 0.01)
		{
			return;
		}
		Log::info("Delay Counter : " + std::to_string(delayCounter));
		delayCounter = 0.0f;
		delayDraw = false;
	}
	Vector2 origin{ texWidth / 2.0f, texHeight / 2.0f };
	renderer.drawSprite(owner, texture, Rectangle::nullRect, origin, IRenderer::Flip::None);
}

void SpriteComponent::setDelayDraw(bool delayDrawP)
{
	delayDraw = delayDrawP;
}