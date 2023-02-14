#pragma once
#include <SDL2/SDL_stdinc.h>
#include "Actor.h"

class Ship : public Actor
{
public:
	Ship();

	void actorInput(const Uint8* keyState) override;
	void updateActor(float dt) override;

	CircleCollisionComponent& getCollision() { return *circleCollision; }

private:
	CircleCollisionComponent* circleCollision;
	SpriteComponent* sc;
	float laserCooldown;
	int hitPoints = 3;
};