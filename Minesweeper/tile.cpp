#include "tile.h"

// Constructors

Tile::Tile(bool hasMine) : hasMine(hasMine), state(State::hidden), minesNear(0) {}

// Will only work for square tiles
Tile::Tile(bool hasMine, const std::shared_ptr<sf::Texture>& texture) 
    : hasMine(hasMine), state(State::hidden), minesNear(0)
{
    sf::Vector2u size = (*texture).getSize();
    this->textureSize = sf::Vector2i(size.y, size.y);
    this->sprite = sf::Sprite(*texture);
    this->changeTextureNumber(0); // TODO: Remove this
}

Tile::Tile(bool hasMine, const std::shared_ptr<sf::Texture>& texture, const sf::Vector2i size) 
    : hasMine(hasMine), state(State::hidden), minesNear(0), textureSize(size)
{
    this->sprite = sf::Sprite(*texture);
    this->changeTextureNumber(0); // TODO: Remove this
}

// Sets mines near to the number of mines in a vector of tiles next to the tile
void Tile::setMinesNear(const std::vector<Tile*>& tilesNear)
{
    this->minesNear = 0;
    for (const auto tile : tilesNear)
        if (tile->getMine())
            minesNear++;
}

// Changes state depending on existing state
void Tile::changeState(const State& newState)
{
    switch (this->state)
    {
    case State::hidden:
        this->state = newState;
        break;
    // Stop from flagging shown tiles
    case State::shown:
        if (newState != State::flagged)
            this->state = newState;
        break;
    // Stop from flagging twice 
    case State::flagged:
        if (newState == State::flagged)
            this->state = State::hidden;
    default:
        break;
    }
}

// Draws tile to screen
void Tile::displayTile(sf::RenderWindow& window, const sf::Vector2f& location)
{
    this->sprite.setPosition(location);
    window.draw(this->sprite);
}

// Set sprite texture to the sprite number on a sprite sheet
void Tile::changeTextureNumber(int textureNumber)
{
    // Position is equal to the textureSize * the textureNumber
    sf::Vector2i position = sf::Vector2i(this->textureSize.x * textureNumber, 0);
    sf::IntRect rect = sf::IntRect(position, this->textureSize);
    this->sprite.setTextureRect(rect);
}

// Updates sprite according to internal state
void Tile::updateSprite()
{
    /*
        0: Empty
        1-8: Numbered
        9: Empty shown
        10: Mine
        11: Flag
    */
    int textureNumber = 0;
    switch (this->state)
    {
    case State::hidden:
        textureNumber = 0;
        break;
    case State::shown:
        if (this->hasMine)
            textureNumber = 10; // TODO: Move this somewhere else
        else if (this->minesNear == 0)
            textureNumber = 9;
        else
            textureNumber = this->minesNear;
        break;
    case State::flagged:
        textureNumber = 11; // TODO: Move this somewhere else
        break;
    default:
        break;
    }
    this->changeTextureNumber(textureNumber);
}