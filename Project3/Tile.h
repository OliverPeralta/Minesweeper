#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "TextureManager.h"
using namespace std;
class Tile
{
public:
	sf::Sprite tile;
	sf::Sprite flag;
	sf::Sprite underTile;
	bool leftClick;
	bool flagged;
	bool isBomb;
	int xPos = 0;
	int yPos = 0;
	unsigned int adjacentMineCount;

	vector<Tile> adjacentTiles;

	Tile();
	Tile(string image);

	sf::Sprite& getSprite();
	void changeSprite(string image);
	void changeUnderSprite(string image);
	void setXY(float x, float y);
	int getX();
	int getY();
	sf::Sprite& getFlag();
	sf::Sprite& getUnderTile();
	void flagCheck();
	bool flaggedCheck();
	void toggleLeftClick();
	bool leftClickCheck();
	void toggleBomb();
	bool checkIfBomb();
	void leftClickOff();
	void flagOff();
	void findAdjacentTiles(vector<Tile> x);
};