#include "game.h"

// TODO: Change playing to menu
Game::Game(sf::RenderWindow& window, int width, int height, sf::Texture& texture) :
    //tileSet(texture),
    gameState(GameState::playing), 
    board(Board(width, height, texture)),
    keyboardMove(sf::Vector2f(0, 0)),
    mouseMove(sf::Vector2f(0, 0)),
    doMouseMove(false),
    currentMousePosition(sf::Vector2f(0, 0)),
    previousMousePosition(sf::Vector2f(0, 0)),
    window(window)
{
    board.setMinesNear();
    board.updateTileSprites();
    board.revealStart();
}

// TODO: Change playing to menu
Game::Game(sf::RenderWindow& window, Board board) :
    gameState(GameState::playing),
    board(board),
    keyboardMove(sf::Vector2f(0, 0)),
    mouseMove(sf::Vector2f(0, 0)),
    doMouseMove(false),
    currentMousePosition(sf::Vector2f(0, 0)),
    previousMousePosition(sf::Vector2f(0, 0)),
    window(window)
{
    board.setMinesNear();
    board.updateTileSprites();
    board.revealStart();
}

// Handles drawing and per-frame caluclations depending on state
void Game::handleFrame()
{
    this->window.clear(sf::Color::Color(240, 240, 240, 255));

    switch (this->gameState)
    {
    case GameState::playing:
        if (this->doMouseMove)
        {
            this->mouseMove = sf::Vector2f(0, 0);
            // -= for opposite vector direction
            this->mouseMove -= this->previousMousePosition - this->currentMousePosition;
        }

        this->previousMousePosition = this->currentMousePosition;

        // Board operations
        this->board.moveBoard(this->M_GLOBAL_MOVE *
                          ((this->M_KEYBOARD_MOVE * this->keyboardMove) +
                              (this->M_MOUSE_MOVE * this->mouseMove)));
        this->board.drawBoard(this->window);
        break;
    default:
        break;
    }

    this->window.display();
}

// Handles event depending on state
void Game::handleEvent(sf::Event event)
{
    switch (this->gameState)
    {
    case GameState::playing:
        this->handlePlaying(event);
        break;
    }
}

// Handles events for when playing the game
void Game::handlePlaying(sf::Event event)
{
    // TODO: Switch to .x += instead of making a new vector
    switch (event.type)
    {
    // Keyboard
    case sf::Event::KeyPressed:
        switch (event.key.code)
        {
        case sf::Keyboard::W:
            // Opposite for vertical directions
            this->keyboardMove += sf::Vector2f(0.0f, 1.0f);
            break;
        case sf::Keyboard::A:
            this->keyboardMove += sf::Vector2f(1.0f, 0.0f);
            break;
        case sf::Keyboard::S:
            // Opposite for vertical directions
            this->keyboardMove += sf::Vector2f(0.0f, -1.0f);
            break;
        case sf::Keyboard::D:
            this->keyboardMove += sf::Vector2f(-1.0f, 0.0f);
            break;
        default:
            break;
        }
        break;

    case sf::Event::KeyReleased:
        switch (event.key.code)
        {
        case sf::Keyboard::W:
            // Opposite for vertical directions
            this->keyboardMove -= sf::Vector2f(0.0f, 1.0f);
            break;
        case sf::Keyboard::A:
            this->keyboardMove -= sf::Vector2f(1.0f, 0.0f);
            break;
        case sf::Keyboard::S:
            // Opposite for vertical directions
            this->keyboardMove -= sf::Vector2f(0.0f, -1.0f);
            break;
        case sf::Keyboard::D:
            this->keyboardMove -= sf::Vector2f(-1.0f, 0.0f);
            break;
        default:
            break;
        }
        break;

    // Mouse
    case sf::Event::MouseButtonPressed:
        switch (event.mouseButton.button)
        {
        case sf::Mouse::Left:
            if (this->board.revealTile(sf::Vector2i((int)event.mouseButton.x, (int)event.mouseButton.y)))
                this->gameState = GameState::end;
            break;
        case sf::Mouse::Right:
            this->board.flagTile(sf::Vector2i((int)event.mouseButton.x, (int)event.mouseButton.y));
            break;
        case sf::Mouse::Middle:
            this->currentMousePosition = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
            this->doMouseMove = true;
            break;
        default:
            break;
        }
        break;

    case sf::Event::MouseButtonReleased:
        switch (event.mouseButton.button)
        {
        case sf::Mouse::Middle:
            this->mouseMove = sf::Vector2f(0, 0);
            this->doMouseMove = false;
            break;
        default:
            break;
        }
        break;

    case sf::Event::MouseMoved:
        this->currentMousePosition = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
        break;

    default:
        break;
    }
}