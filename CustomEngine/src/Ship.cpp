#include "Assets.h"
#include "Game.h"
#include "InputComponent.h"
#include "Laser.h"
#include "Log.h"
#include "Maths.h"
#include "Ship.h"
#include "SpriteComponent.h"

Ship::Ship() : Actor(), laserCooldown(0.0f)
{
	sc = new SpriteComponent(this, Assets::getTexture("Ship"));
	InputComponent* ic = new InputComponent(this);

	ic->setMaxForwardSpeed(300.0f);
	ic->setMaxAngularSpeed(Maths::twoPi);

	circleCollision = new CircleCollisionComponent(this);
	circleCollision->setRadius(40.0f);
}

void Ship::actorInput(const Uint8* keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] && laserCooldown <= 0.0f)
	{
		Laser* laser = new Laser();
		laser->setPosition(getPosition());
		laser->setRotation(getRotation());
		laserCooldown = 0.2f;
	}
}

void Ship::updateActor(float dt)
{
	laserCooldown -= dt;
	auto asteroids = getGame().getAsteroids();
	for (auto asteroid : asteroids)
	{
		if (Intersect(*circleCollision, asteroid->getCollision()))
		{
			Log::info("Collided with Asteroid");
			if (hitPoints <= 0)
				setState(ActorState::Dead);

			hitPoints--;
			sc->setDelayDraw(true);
			setPosition(Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));

			asteroid->setState(ActorState::Dead);
			break;
		}
	}
}