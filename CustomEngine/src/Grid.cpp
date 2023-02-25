#include <algorithm>

#include "Grid.h"
#include "Tile.h"

Grid::Grid() : Actor(), selectedTile(nullptr), nextEnemyTimer(0)
{
	// 7 rows, 16 columns
	tiles.resize(NB_ROWS);
	for (size_t i = 0; i < tiles.size(); i++)
	{
		tiles[i].resize(NB_COLS);
	}

	// Create tiles
	for (size_t i = 0; i < NB_ROWS; i++)
	{
		for (size_t j = 0; j < NB_COLS; j++)
		{
			tiles[i][j] = new Tile();
			tiles[i][j]->setPosition(
				Vector2(TILESIZE / 2.0f + j * TILESIZE, START_Y + i * TILESIZE));
			// Create obstacle in the middle of the grid
			if (i >= NB_ROWS / 2 - 1 && i <= NB_ROWS / 2 + 1 &&
				j >= NB_COLS / 2 - 1 && j <= NB_COLS / 2 + 1)
			{
				tiles[i][j]->setTileState(Tile::TileState::Obstacle);
			}
			else
			{
				tiles[i][j]->setTileState(Tile::TileState::Default);
			}
		}
	}

	// Set start/end tiles
	getStartTile().setTileState(Tile::TileState::Start);
	getEndTile().setTileState(Tile::TileState::Base);

	// Fill adjacent tiles
	for (size_t i = 0; i < NB_ROWS; i++)
	{
		for (size_t j = 0; j < NB_COLS; j++)
		{
			if (i > 0)
				tiles[i][j]->adjacentTiles.push_back(tiles[i - 1][j]);
			if (i < NB_ROWS - 1)
				tiles[i][j]->adjacentTiles.push_back(tiles[i + 1][j]);
			if (j > 0)
				tiles[i][j]->adjacentTiles.push_back(tiles[i][j - 1]);
			if (j < NB_COLS - 1)
				tiles[i][j]->adjacentTiles.push_back(tiles[i][j + 1]);
		}
	}

	// Find path in reverse
	findPath(getEndTile(), getStartTile());
	updatePathTiles(getStartTile());
}

void Grid::processClick(int x, int y)
{
	y -= static_cast<int>(START_Y - TILESIZE / 2);
	if (y >= 0)
	{
		x /= static_cast<int>(TILESIZE);
		y /= static_cast<int>(TILESIZE);
		if (x >= 0 && x < static_cast<int>(NB_COLS) &&
			y >= 0 && y < static_cast<int>(NB_ROWS))
		{
			selectTile(y, x);
		}
	}
}

Tile& Grid::getStartTile()
{
	return *tiles[3][0];
}

Tile& Grid::getEndTile()
{
	return *tiles[3][15];
}

void Grid::selectTile(size_t row, size_t col)
{
}

bool Grid::findPath(Tile& start, const Tile& goal)
{
	for (auto i = 0; i < NB_ROWS; i++)
	{
		for (auto j = 0; j < NB_COLS; j++)
		{
			tiles[i][j]->gCost = 0.0f;
			tiles[i][j]->isInOpenSet = false;
			tiles[i][j]->isInClosedSet = false;
		}
	}

	vector<Tile*> openSet;
	Tile* current = &start;
	current->isInClosedSet = true;

	do
	{
		// Add adjacent nodes to open set
		for (Tile* neighbour : current->adjacentTiles)
		{
			if (neighbour->isBlocked)
			{
				continue;
			}

			// Only check a node if it is not in the closed set
			if (!neighbour->isInClosedSet)
			{
				if (!neighbour->isInOpenSet)
				{
					// Not in open set, so set parent
					neighbour->parent = current;
					neighbour->hCost = (neighbour->getPosition() - goal.getPosition()).length();
					// gCost = parent's gCost + cost of traversing edge
					neighbour->gCost = current->gCost + TILESIZE;
					neighbour->fCost = neighbour->gCost + neighbour->hCost;
					openSet.emplace_back(neighbour);
					neighbour->isInOpenSet = true;
				}
				else
				{
					// Compute g cost if current becomes the parent
					float newGCost = current->gCost + TILESIZE;
					if (newGCost < neighbour->gCost)
					{
						// Adopt this node
						neighbour->parent = current;
						neighbour->gCost = newGCost;
						neighbour->fCost = neighbour->gCost + neighbour->hCost;
					}
				}
			}
		}

		// If open set is empty, all possible paths are exhausted
		if (openSet.empty())
		{
			break;
		}

		// Find lowest cost node in open set, the second line is a lambda function
		auto iter = std::min_element(begin(openSet), end(openSet),
			[](Tile* a, Tile* b) {return a->fCost < b->fCost; });

		// Set to current and move from open to closed set
		current = *iter;
		openSet.erase(iter);
		current->isInOpenSet = false;
		current->isInClosedSet = true;
	} while (current != &goal);
	
	return (current == &goal);
}

void Grid::updatePathTiles(const Tile& start)
{
	Tile* t = start.parent;
	while (t != &getEndTile())
	{
		t->setTileState(Tile::TileState::Path);
		t = t->parent;
	}
}

void Grid::updateActor(float dt)
{
	Actor::updateActor(dt);
	nextEnemyTimer -= dt;
	if (nextEnemyTimer <= 0.0f)
	{
		new Enemy();
		nextEnemyTimer += TIME_BETWEEN_ENEMIES;
	}
}