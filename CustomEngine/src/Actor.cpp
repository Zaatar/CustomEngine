#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include "Maths.h"

#include <algorithm>

Actor::Actor() :
	state(Actor::ActorState::Active),
	position(Vector2::zero),
	scale(1.0f),
	rotation(0.0f),
	mustRecomputeWorldTransform(true),
	game(Game::instance())
{
	game.addActor(this);
}

Actor::~Actor()
{
	game.removeActor(this);
	// Need to delete components
	// Because ~Component calls RemoveComponent, needs a different style loop
	while (!components.empty())
	{
		delete components.back();
	}
}

Vector2 Actor::getForward() const
{
	return Vector2(Maths::cos(rotation), Maths::sin(rotation));
	// Positive in open GL negative in SDL renderer
}

void Actor::setState(ActorState stateP)
{
	state = stateP;
}

void Actor::setPosition(Vector2 positionP)
{
	position = positionP;
	mustRecomputeWorldTransform = true;
}

void Actor::setScale(float scaleP)
{
	scale = scaleP;
	mustRecomputeWorldTransform = true;
}

void Actor::setRotation(float rotationP)
{
	rotation = rotationP;
	mustRecomputeWorldTransform = true;
}

void Actor::update(float dt)
{
	if (state == Actor::ActorState::Active)
	{
		updateComponents(dt);
		updateActor(dt);
	}
}

void Actor::updateComponents(float dt)
{
	for (auto component : components)
	{
		component->update(dt);
	}
}

void Actor::updateActor(float dt)
{

}

void Actor::addComponent(Component* component)
{
	// Find the insertion point in the sorted vector
	// (The first element with an order higher than me)
	int myOrder = component->getUpdateOrder();
	auto iter = begin(components);
	for (; iter != end(components); ++iter)
	{
		if (myOrder < (*iter)->getUpdateOrder())
		{
			break;
		}
	}

	// Inserts elements before position of iterator
	components.insert(iter, component);
}

void Actor::removeComponent(Component* component)
{
	auto iter = std::find(begin(components), end(components), component);
	if (iter != end(components))
	{
		components.erase(iter);
	}
}

void Actor::processInput(const Uint8* keyState)
{
	if (state == Actor::ActorState::Active)
	{
		for (auto component : components)
		{
			component->processInput(keyState);
		}
		actorInput(keyState);
	}
}

void Actor::actorInput(const Uint8* keyState) {}

void Actor::computeWorldTransform()
{
	if (mustRecomputeWorldTransform)
	{
		mustRecomputeWorldTransform = false;
		worldTransform = Matrix4::createScale(scale);
		worldTransform *= Matrix4::createRotationZ(rotation);
		worldTransform *= Matrix4::createTranslation(Vector3(position.x, position.y, 0.0f));

		for (auto component : components)
		{
			component->onUpdateWorldTransform();
		}
	}
}