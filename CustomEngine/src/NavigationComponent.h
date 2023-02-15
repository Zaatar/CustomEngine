#pragma once
#include "MoveComponent.h"
#include "Vector2.h"

class NavigationComponent : public MoveComponent
{
public:
	NavigationComponent(class Actor* owner, int updateOrder = 10);
	NavigationComponent() = delete;
	NavigationComponent(const NavigationComponent&) = delete;
	NavigationComponent& operator=(const NavigationComponent&) = delete;

	void update(float dt) override;
	void startPath(const class Tile& start);
	void turnTo(const Vector2& targetPosition);

private:
	const class Tile* nextNode;
};