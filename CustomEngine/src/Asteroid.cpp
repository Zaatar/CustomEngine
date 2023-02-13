#include "Assets.h"
#include "Asteroid.h"
#include "Game.h"
#include "Maths.h"
#include "MoveComponent.h"
#include "Random.h"
#include "SpriteComponent.h"
#include "Window.h"

Asteroid::Asteroid() : Actor()
{
	Vector2 randPos = Random::getVector(Vector2::zero, Vector2(WINDOW_WIDTH, WINDOW_HEIGHT));
	setPosition(randPos);
	setRotation(Random::getFloatRange(0.0f, Maths::twoPi));

	SpriteComponent* sc = new SpriteComponent(this, Assets::getTexture("Asteroid"));
	MoveComponent* mc = new MoveComponent(this);
	mc->setForwardSpeed(150.0f);

	collision = new CircleCollisionComponent(this);
	collision->setRadius(40.0f);

	getGame().addAsteroid(this);
}

Asteroid::~Asteroid()
{
	getGame().removeAsteroid(this);
}
