#include "Assets.h"
#include "Tile.h"

Tile::Tile() :
	Actor(),
	sprite(nullptr),
	tileState(TileState::Default),
	isSelected(false),
	isBlocked(false),
	parent(nullptr),
	fCost(0),
	gCost(0),
	hCost(0),
	isInOpenSet(false),
	isInClosedSet(false)
{
	sprite = new SpriteComponent(this, Assets::getTexture("TileBrown"));
}

Tile::Tile(const Tile& tileP) :
	Actor(),
	sprite(nullptr),
	tileState(tileP.tileState),
	isSelected(tileP.isSelected),
	isBlocked(tileP.isBlocked),
	parent(tileP.parent),
	fCost(tileP.fCost),
	gCost(tileP.gCost),
	hCost(tileP.hCost),
	isInOpenSet(tileP.isInOpenSet),
	isInClosedSet(tileP.isInClosedSet)
{
	sprite = new SpriteComponent(this, Assets::getTexture("TileBrown"));
}

Tile Tile::operator=(const Tile& tileP)
{
	Tile t;
	t.sprite = new SpriteComponent(this, Assets::getTexture("TileBrown"));
	t.tileState = tileP.tileState;
	t.isSelected = tileP.isSelected;
	t.isBlocked = tileP.isBlocked;
	t.parent = tileP.parent;
	t.fCost = tileP.fCost;
	t.gCost = tileP.gCost;
	t.hCost = tileP.hCost;
	t.isInOpenSet = tileP.isInOpenSet;
	t.isInClosedSet = tileP.isInClosedSet;
	return t;
}

void Tile::setTileState(TileState tileStateP)
{
	tileState = tileStateP;
	updateTexture();
}

void Tile::toggleSelect()
{
	isSelected = !isSelected;
	updateTexture();
}

void Tile::updateTexture()
{
	std::string text;
	switch (tileState)
	{
	case TileState::Start:
		text = "TileTan";
		break;
	case TileState::Base:
		text = "TileGreen";
		break;
	case TileState::Path:
		if (isSelected)
			text = "TileGreySelected";
		else
			text = "TileGrey";
		break;
	case TileState::Obstacle:
		isBlocked = true;
		if (isSelected)
			text = "TileRedSelected";
		else
			text = "TileRed";
		break;
	case TileState::Default:
	default:
		if (isSelected)
			text = "TileBrownSelected";
		else
			text = "TileBrown";
		break;
	}
	sprite->setTexture(Assets::getTexture(text));
}