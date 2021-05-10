#include <SFML/Graphics.hpp>

#include "board.h"
#include "tile.h"

int main()
{
    // Setup Render Window
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Minesweeper");
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(75);

    // Setup Tile Texture
    std::shared_ptr<sf::Texture> tileTexture = std::make_shared<sf::Texture>();
    tileTexture->loadFromFile("tileset.png");

    // Setup Board
    Board board(20, 20, tileTexture);
    board.setMinesNear();
    board.updateTileSprites();

    // Input States
    sf::Vector2f unitMove(0, 0);
    sf::Vector2f mouseMove(0, 0);
    bool doMoveMouse = false;
    const float MOVE_AMOUNT = 1.5f;

    sf::Vector2f currentMousePosition(0, 0);
    sf::Vector2f previousMousePosition(0, 0);

    // Game Loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            // Keyboard
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::W:
                    // Opposite for vertical directions
                    unitMove += sf::Vector2f(0.0f, 1.0f);
                    break;
                case sf::Keyboard::A:
                    unitMove += sf::Vector2f(1.0f, 0.0f);
                    break;
                case sf::Keyboard::S:
                    // Opposite for vertical directions
                    unitMove += sf::Vector2f(0.0f, -1.0f);
                    break;
                case sf::Keyboard::D:
                    unitMove += sf::Vector2f(-1.0f, 0.0f);
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
                    unitMove -= sf::Vector2f(0.0f, 1.0f);
                    break;
                case sf::Keyboard::A:
                    unitMove -= sf::Vector2f(1.0f, 0.0f);
                    break;
                case sf::Keyboard::S:
                    // Opposite for vertical directions
                    unitMove -= sf::Vector2f(0.0f, -1.0f);
                    break;
                case sf::Keyboard::D:
                    unitMove -= sf::Vector2f(-1.0f, 0.0f);
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
                    board.revealTile(sf::Vector2i((int)event.mouseButton.x, (int)event.mouseButton.y));
                    break;
                case sf::Mouse::Right:
                    board.flagTile(sf::Vector2i((int)event.mouseButton.x, (int)event.mouseButton.y));
                    break;
                case sf::Mouse::Middle:
                    currentMousePosition = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                    doMoveMouse = true;
                    break;
                default:
                    break;
                }
                break;

            case sf::Event::MouseButtonReleased:
                mouseMove = sf::Vector2f(0, 0);
                doMoveMouse = false;
                break;

            case sf::Event::MouseMoved:
                currentMousePosition = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
                break;

            default:
                break;
            }
        }

        if (doMoveMouse)
        {
            mouseMove = sf::Vector2f(0, 0);
            // -= for opposite vector direction
            mouseMove -= previousMousePosition - currentMousePosition;
        }
        
        previousMousePosition = currentMousePosition;

        // Drawing
        window.clear(sf::Color::Color(240, 240, 240, 255));

        // Board operations
        board.moveBoard(MOVE_AMOUNT * (unitMove + mouseMove));
        board.drawBoard(window);

        window.display();
    }

    return 0;
}