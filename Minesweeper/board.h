#pragma once

#include <iostream>
#include <vector>
#include <random>

#include "tile.h"

/*
    Board class
    boardPosition: Position of the board
    tiles: The 2 dimensional array of Tiles
*/
class Board
{
public:
    // Constructors/Destructors
    Board();
    Board(int, int, std::shared_ptr<sf::Texture>);

    // Tile operations
    void createBoard(int, int, std::shared_ptr<sf::Texture>);
    void setMinesNear();
    void updateTileSprites();
    void changeTile(int, int, State);

    // Board operations
    void moveBoard(const sf::Vector2f& amount) { this->boardPosition += amount; }

    // Screen operations
    sf::Vector2i getTile(const sf::Vector2i&);
    void revealTile(const sf::Vector2i&);
    void flagTile(const sf::Vector2i&);

    // Getters
    int getBoardWidth() { return this->tiles.size(); };
    int getBoardHeight() { return this->tiles.front().size(); };
    const sf::Vector2i& getTileSize() { return this->tiles.at(0).at(0).getTextureSize(); };

    // Sprites
    void drawBoard(sf::RenderWindow&);

private:
    sf::Vector2f boardPosition;
    std::vector<std::vector<Tile>> tiles;
};

