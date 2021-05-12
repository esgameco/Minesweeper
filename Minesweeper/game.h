#pragma once

#include "board.h"

enum class GameState
{
	menu,
	playing,
	paused,
	end
};

class Game
{
public:
	// Constructors
	Game(sf::RenderWindow&, int, int, sf::Texture&);
	Game(sf::RenderWindow&, Board);

	// Handlers
	void handleFrame();
	void handleEvent(sf::Event);
	void handlePlaying(sf::Event);

private:
	// Constant multipliers
	const float M_GLOBAL_MOVE = 1.5f;
	const float M_MOUSE_MOVE = 1.0f;
	const float M_KEYBOARD_MOVE = 10.0f;

	// Textures
	//sf::Texture tileSet;

	// Game State
	Board board;
	GameState gameState;

	// Input State
	sf::Vector2f keyboardMove;
	sf::Vector2f mouseMove;

	bool doMouseMove;
	sf::Vector2f currentMousePosition;
	sf::Vector2f previousMousePosition;

	sf::RenderWindow& window;
};