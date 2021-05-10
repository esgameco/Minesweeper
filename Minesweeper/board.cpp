#include "board.h"
#include "tile.h"

Board::Board()
{
}

Board::Board(int width, int height, std::shared_ptr<sf::Texture> texture)
{
    createBoard(width, height, texture);
}

// Creates a board 
void Board::createBoard(int width, int height, std::shared_ptr<sf::Texture> texture)
{
    // Random number generator
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<> dist(0, 0.6); // TODO: Move the 0.6 somewhere else

    this->tiles.reserve(width);
    for (int i = 0; i < width; i++)
    {
        std::vector<Tile> newVector;
        for (int j = 0; j < height; j++)
        {
            // Decides whether to have a mine or not by  
            // rounding a double to 1 (true) or 0 (false)
            bool hasMine = static_cast<bool>(std::round(dist(rng)));
            Tile newTile(hasMine, texture);
            newVector.push_back(newTile);
        }
        this->tiles.push_back(newVector);
    }
}

// Calculates and stores the mines near each tile
void Board::setMinesNear()
{
    // Tile mapping
    // TODO: Maybe add out of this scope for performance
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

    for (int i = 0; i < this->getBoardWidth(); i++)
    {
        for (int j = 0; j < this->getBoardHeight(); j++)
        {
            std::vector<Tile*> tilesNear;
            for (const auto& pos : tilesNearPositions)
            {
                try
                {
                    tilesNear.push_back(&(this->tiles.at(pos.x+i).at(pos.y+j)));
                }
                // Excepts if out of range of the board
                catch (...) {} // Do nothing
            }
            this->tiles[i][j].setMinesNear(tilesNear);
        }
    }
}

// Updates all sprites on the board to reflect their state
void Board::updateTileSprites()
{
    for (int i = 0; i < this->getBoardWidth(); i++)
    {
        for (int j = 0; j < this->getBoardHeight(); j++)
        {
            this->tiles[i][j].updateSprite();
        }
    }
}

// Updates a specific tile to a state
void Board::changeTile(int x, int y, State state)
{
    try
    {
        Tile& tile = this->tiles.at(x).at(y);
        tile.changeState(state);
        tile.updateSprite();
    }
    catch(...) {}
}

// Gets the tile at screen coordinates
sf::Vector2i Board::getTile(const sf::Vector2i& screenCoords)
{
    // Needs tile size to fit coords to tiles
    const sf::Vector2i tileSize = this->getTileSize();
    // Starts with coords normalized to (0,0)
    sf::Vector2i tileCoords = screenCoords - sf::Vector2i((int)this->boardPosition.x, (int)this->boardPosition.y);

    tileCoords.x /= tileSize.x;
    tileCoords.y /= tileSize.y;

    return tileCoords;
}

// Reveals tile at a specific screen location
// (Necessary to remove State access from main)
// TODO: Use templates for this
void Board::revealTile(const sf::Vector2i& screenCoords)
{
    sf::Vector2i tileCoords = this->getTile(screenCoords);

    this->changeTile(tileCoords.x, tileCoords.y, State::shown);
}

// Flags tile at a specific screen location
// (Necessary to remove State access from main)
void Board::flagTile(const sf::Vector2i& screenCoords)
{
    sf::Vector2i tileCoords = this->getTile(screenCoords);

    this->changeTile(tileCoords.x, tileCoords.y, State::flagged);
}

// Draws board to screen
// Board must contain at least one element to draw
void Board::drawBoard(sf::RenderWindow& window)
{
    for (int i = 0; i < this->getBoardWidth(); i++)
    {
        for (int j = 0; j < this->getBoardHeight(); j++)
        {
            this->tiles[i][j].displayTile(window, this->boardPosition+sf::Vector2f(i*64, j*64));
        }
    }
}