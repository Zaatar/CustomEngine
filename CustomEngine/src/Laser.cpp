#include "Assets.h"
#include "Asteroid.h"
#include "Game.h"
#include "Laser.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"

Laser::Laser() :
	Actor(), deathTimer(1.0f), collision(nullptr)
{
	new SpriteComponent(this, Assets::getTexture("Laser"));
	MoveComponent* mc = new MoveComponent(this);
	mc->setForwardSpeed(800.0f);
	collision = new CircleCollisionComponent(this);
	collision->setRadius(11.0f);
}

void Laser::updateActor(float dt)
{
	deathTimer -= dt;
	if (deathTimer <= 0.0f)
	{
		setState(Actor::ActorState::Dead);
	}
	else
	{
		auto asteroids = getGame().getAsteroids();
		for (auto asteroid : asteroids)
		{
			if (Intersect(*collision, asteroid->getCollision()))
			{
				setState(ActorState::Dead);
				asteroid->setState(ActorState::Dead);
				break;
			}
		}
	}
}