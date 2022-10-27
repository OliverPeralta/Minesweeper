#include "Tile.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;


Tile::Tile() {
	tile.setTexture(TextureManager::GetTexture("tile_hidden"));
	flag.setTexture(TextureManager::GetTexture("flag"));;
	underTile.setTexture(TextureManager::GetTexture("tile_revealed"));
	flagged = false;
	leftClick = false;
	isBomb = false;
}
Tile::Tile(string image) {
	tile.setTexture(TextureManager::GetTexture(image));
	flag.setTexture(TextureManager::GetTexture("flag"));
	underTile.setTexture(TextureManager::GetTexture("tile_revealed"));
	flagged = false;
	leftClick = false;
	isBomb = false;
}
sf::Sprite& Tile::getSprite() {
	return tile;
}
void Tile::changeSprite(string image) {
	tile.setTexture(TextureManager::GetTexture(image));
}
void Tile::changeUnderSprite(string image) {
	underTile.setTexture(TextureManager::GetTexture(image));
}
void Tile::setXY(float x, float y) {
	xPos = x;
	yPos = y;
}
int Tile::getX() {
	return xPos;
}
int Tile::getY() {
	return yPos;
}
sf::Sprite& Tile::getFlag() {
	return flag;
}
void Tile::flagCheck() {
	if (flagged)
		flagged = false;
	else
		flagged = true;
}
bool Tile::flaggedCheck() {
	return flagged;
}
sf::Sprite& Tile::getUnderTile() {
	return underTile;
}
void Tile::toggleLeftClick() {
	if (leftClick)
		leftClick = false;
	else
		leftClick = true;
}
void Tile::leftClickOff() {
	leftClick = false;
}
bool Tile::leftClickCheck() {
	return leftClick;
}
void Tile::toggleBomb() {
	if (isBomb)
		isBomb = false;
	else {
		underTile.setTexture(TextureManager::GetTexture("tile_hidden"));
		adjacentMineCount = 9;
		isBomb = true;
	}
}
bool Tile::checkIfBomb() {
	adjacentMineCount = 9;
	return isBomb;
}

void Tile::flagOff() {
	flagged = false;
}

void Tile::findAdjacentTiles(vector<Tile> x) {
	adjacentTiles = x;
	adjacentMineCount = adjacentTiles.size();
}

