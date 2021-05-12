#include <SFML/Graphics.hpp>

#include "game.h"
#include "board.h"
#include "tile.h"

int main()
{
    // Constants
    const int WIDTH = 20;
    const int HEIGHT = 20;

    // Setup Render Window
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Minesweeper");
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(75);

    // Setup Tile Texture
    sf::Texture tileTexture = sf::Texture();
    tileTexture.loadFromFile("tileset.png");

    // Setup Game
    Game game(window, WIDTH, HEIGHT, tileTexture);

    // Game Loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Closed should be outside of the game scope
            if (event.type == sf::Event::Closed)
            {
                window.close();
                // TODO: Find another way to exit cleanly
                return 0;
            }
            else
            {
                // Game is in charge of handling the events
                game.handleEvent(event);
            }
        }

        // Handle frame manages things that are done every frame like movement
        game.handleFrame();
    }

    return 0;
}