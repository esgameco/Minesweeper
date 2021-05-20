#pragma once

#include <iostream>
#include <vector>
#include <random>

#include "tile.h"

// TODO: Figure out the best way to store this
// TODO: Capitalize
const sf::Vector2i tilesNearPositions[8] =
{
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, -1},
    {0, 1},
    {1, -1},
    {1, 0},
    {1, 1}
};

/*
    Board class

    MINE_CHANCE: The chance of a mine occurring

    boardPosition: Position of the board
    tiles: The 2 dimensional array of Tiles
*/
class Board
{
public:
    // Constructors/Destructors
    Board();
    Board(int, int, sf::Texture&);
    void newBoard(int, int, sf::Texture&);

    // Tile operations
    void createBoard(int, int, sf::Texture&);
    void setMinesNear();
    void updateTileSprites();
    void changeTile(int, int, State);
    void revealAround(const sf::Vector2i&, std::vector<sf::Vector2i>&);
    void revealAround(const sf::Vector2i&); // Recursive handler
    void revealStart();
    void revealAll();
    bool checkMine(const sf::Vector2i&, bool);

    // Board operations
    void moveBoard(const sf::Vector2f& amount) { this->boardPosition += amount; };

    // Screen operations
    sf::Vector2i getTile(const sf::Vector2i&);
    bool revealTile(const sf::Vector2i&);
    void flagTile(const sf::Vector2i&);

    // Getters
    const int getBoardWidth() const { return this->tiles.size(); };
    const int getBoardHeight() const { return this->tiles.front().size(); };
    const sf::Vector2i& getTileSize() const { return this->tiles.at(0).at(0).getTextureSize(); };

    // Sprites
    void drawBoard(sf::RenderWindow&);

private:
    // Constants
    const double MINE_CHANCE = 0.1;

    sf::Vector2f boardPosition;
    std::vector<std::vector<Tile>> tiles;
};

