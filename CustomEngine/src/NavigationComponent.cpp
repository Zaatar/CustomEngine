#include "Actor.h"
#include "Maths.h"
#include "NavigationComponent.h"
#include "Tile.h"

NavigationComponent::NavigationComponent(Actor* owner, int updateOrder) :
	MoveComponent(owner, updateOrder), nextNode(nullptr) {}

void NavigationComponent::update(float dt)
{
	if (nextNode)
	{
		Vector2 dist = owner.getPosition() - nextNode->getPosition();
		if (Maths::nearZero(dist.length(), 2.0f))
		{
			nextNode = nextNode->getParent();
			turnTo(nextNode->getPosition());
		}
	}
	MoveComponent::update(dt);
}

void NavigationComponent::startPath(const Tile& start)
{
	nextNode = start.getParent();
	turnTo(nextNode->getPosition());
}

void NavigationComponent::turnTo(const Vector2& targetPosition)
{
	Vector2 direction = targetPosition - owner.getPosition();
	float angle = Maths::atan2(-direction.y, direction.x);
	owner.setRotation(angle);
}