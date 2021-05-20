#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>
#include <numeric>

enum class State
{
    hidden,
    shown,
    flagged
};

/*
    Tile class

    hasMine: Whether the tile has a mine or not
    numberNear: The number of tiles with mines within radius
    state: Whether the tile is hidden, revealed, or flagged

    sprite: The SFML sprite of the tile
    textureSize: The size of each individual texture in a spritesheet
*/
class Tile
{
public:
    // Constructors/Destructors
    Tile(bool);
    Tile(bool, const sf::Texture&);
    Tile(bool, const sf::Texture&, const sf::Vector2i);

    // Tile Operations
    void setMinesNear(const std::vector<Tile*>&);
    void changeState(const State&);
    void changeStateOverride(const State& newState) { this->state = newState; };

    // Getters
    const bool getFlagged() const { return this->state == State::flagged; };
    const bool getMine() const { return this->hasMine; };
    const bool noMinesNear() const { return this->minesNear == 0; };
    const sf::Vector2i& getTextureSize() const { return this->textureSize; };

    // Sprites
    void displayTile(sf::RenderWindow&, const sf::Vector2f&);
    void changeTextureNumber(int);
    void updateSprite();

private:
    bool hasMine;
    int minesNear;
    State state;

    sf::Sprite sprite;
    sf::Vector2i textureSize;
};