#pragma once
#include "Vector2.h"
struct Ball
{
	Vector2 vectorPosition;
	Vector2 vectorVelocity;

	Ball(Vector2 vecPositionP, Vector2 vecVelocityP) : vectorPosition(vecPositionP), vectorVelocity(vecVelocityP) {}

};