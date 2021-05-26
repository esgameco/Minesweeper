#include "board.h"
#include "tile.h"

Board::Board()
{
}

Board::Board(int _width, int _height, sf::Texture& _texture)
{
    this->newBoard(_width, _height, _texture);
}

// Resets the board
void Board::newBoard(int width, int height, sf::Texture& texture)
{
    this->tiles.clear();
    this->createBoard(width, height, texture);
}

// Creates a board 
void Board::createBoard(int width, int height, sf::Texture& texture)
{
    // Random number generator
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<> dist(0, 0.5+MINE_CHANCE); // TODO: Move the constant somewhere else

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

// Recursively reveals tiles from 0-mine tiles
void Board::revealAround(const sf::Vector2i& tileCoord, std::vector<sf::Vector2i>& seenTiles)
{
    bool hasBeenSeen = false;
    seenTiles.push_back(tileCoord);
    try
    {
        Tile& tile = this->tiles.at(tileCoord.x).at(tileCoord.y);
        tile.changeState(State::shown);
        tile.updateSprite();
        if (tile.noMinesNear())
            // Iterates through nearby position map
            for (const auto& pos : tilesNearPositions)
            {
                hasBeenSeen = false;
                for (const auto& seen : seenTiles)
                    // Makes sure that the new coord hasn't been seen
                    // and that it is not the current coord
                    if (tileCoord + pos == seen && tileCoord + pos != tileCoord)
                    {
                        hasBeenSeen = true;
                        break;
                    }
                if (!hasBeenSeen)
                    // Calls itself if the new position hasn't been seen 
                    this->revealAround(tileCoord + pos, seenTiles);
            }
    }
    catch (...) {}
}

// Recursively reveals tiles from 0-mine tiles
// (Shorthand for method above)
void Board::revealAround(const sf::Vector2i& tileCoords)
{
    std::vector<sf::Vector2i> seen;

    this->revealAround(tileCoords, seen);
}

// Reveals random 0-mine tiles at beginning
void Board::revealStart()
{
    const int TRIES = 5000; // TODO: Move constant somewhere else

    // TODO: Wrap into function
    // Random number generator
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<> x(0, this->getBoardWidth()-1); // TODO: Move the constant somewhere else
    std::uniform_int_distribution<> y(0, this->getBoardHeight()-1);

    for (int i = 0; i < TRIES; i++)
    {
        sf::Vector2i coord(x(rng), y(rng));
        if (this->tiles[coord.x][coord.y].noMinesNear() && !this->tiles[coord.x][coord.y].getMine())
        {
            this->revealAround(coord);
            return;
        }
    }
}

// Reveals all tiles
void Board::revealAll()
{
    for (int i = 0; i < this->getBoardWidth(); i++)
    {
        for (int j = 0; j < this->getBoardHeight(); j++)
        {
            this->changeTile(i, j, State::shown);
        }
    }
}

// Checks whether a tile has a mine or not
bool Board::checkMine(const sf::Vector2i& tileCoords, bool onlyIfNotFlagged=false)
{
    try
    {
        Tile& tile = this->tiles.at(tileCoords.x).at(tileCoords.y);
        if (onlyIfNotFlagged)
        {
            if (!tile.getFlagged())
                return tile.getMine();
        }
        else
            return tile.getMine();
    }
    catch (...) {}
    return false;
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
bool Board::revealTile(const sf::Vector2i& screenCoords)
{
    sf::Vector2i tileCoords = this->getTile(screenCoords);

    // Has mine
    if (this->checkMine(tileCoords, true))
    {
        this->revealAll();
        return true;
    }

    // Doesn't have mine
    this->revealAround(tileCoords);
    return false;
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