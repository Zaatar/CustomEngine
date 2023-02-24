#pragma once
#include <SDL2/SDL_stdinc.h>
#include <vector>
#include "Matrix4.h"
#include "Vector2.h"

using std::vector;

class Game;
class Component;

class Actor
{
public:
	enum class ActorState
	{
		Active, Paused, Dead
	};

	Actor();
	virtual ~Actor();
	Actor(const Actor&) = delete;
	Actor& operator=(const Actor&) = delete;

	Game& getGame() const { return game; }
	const ActorState getState() const { return state; }
	const Vector2 getPosition() const { return position; }
	const Matrix4& getWorldTransform() const { return worldTransform; }

	const float getScale() const { return scale; }
	const float getRotation() const { return rotation; }
	Vector2 getForward() const;

	void setState(ActorState stateP);
	void setPosition(Vector2 positionP);
	void setScale(float scaleP);
	void setRotation(float rotationP);

	void update(float dt);
	void updateComponents(float dt);
	virtual void updateActor(float dt);
	void addComponent(Component* component);
	void removeComponent(Component* component);

	void processInput(const Uint8* keyState);
	virtual void actorInput(const Uint8* keyState);

	void computeWorldTransform();

private:
	Game& game;
	ActorState state;
	Vector2 position;
	float scale;
	float rotation; // Rotation in radians

	vector<Component*> components;

	Matrix4 worldTransform;
	bool mustRecomputeWorldTransform;
};