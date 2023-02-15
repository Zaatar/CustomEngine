#pragma once
#include <vector>
#include "Actor.h"
#include "Tile.h"

class Grid : public Actor
{
public:
	Grid();
	
	void processClick(int x, int y);
	class Tile& getStartTile();
	class Tile& getEndTile();

	bool findPath(Tile& start, const Tile& goal);
	void updateActor(float dt) override;

private:
	void selectTile(size_t row, size_t col);
	void updatePathTiles(const Tile& start);
	class Tile* selectedTile;

	// 2D vector of tiles in grid
	std::vector<std::vector<class Tile*>> tiles;

	const size_t NB_ROWS = 7;
	const size_t NB_COLS = 16;

	// Start y position of top left corner
	const float START_Y = 192.0f;
	const float TILESIZE = 64.0f;

	float nextEnemyTimer;
	const float TIME_BETWEEN_ENEMIES = 1.5f;
};