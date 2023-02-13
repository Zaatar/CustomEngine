#pragma once
#include <SDL2/SDL_stdinc.h>
#include "Actor.h"

class Ship : public Actor
{
public:
	Ship();

	void actorInput(const Uint8* keyState) override;
	void updateActor(float dt) override;

private:
	float laserCooldown;
};