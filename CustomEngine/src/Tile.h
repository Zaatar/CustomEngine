#pragma once
#include <vector>

#include "Actor.h"
#include "SpriteComponent.h"

class Tile : public Actor
{
public:
	friend class Grid;
	enum class TileState
	{
		Default,
		Path,
		Start,
		Base,
		Obstacle
	};

	Tile();
	Tile(const Tile&);
	Tile operator=(const Tile&);

	TileState getTileState() const { return tileState; }
	const Tile* getParent() const { return parent; }
	void setTileState(TileState tileStateP);

	void toggleSelect();

private:
	void updateTexture();
	SpriteComponent* sprite;
	TileState tileState;
	bool isSelected = false;
	bool isBlocked = false;

	// Pathfinding
	std::vector<Tile*> adjacentTiles;
	Tile* parent;
	float gCost, hCost, fCost;
	bool isInOpenSet, isInClosedSet;
};