#pragma once

#include "board.h"
#include "menu.h"

enum class GameState
{
	menu,
	playing,
	paused,
	end
};

/*
	Game class

	tileSet: Tileset texture

	board: Game board
	gameState: Game state
	menu: Game menu

	keyboardMove: Direction in which camera should move based on keyboard
	mouseMove: Direction in which camera should move based on mouse

	doMouseMove: Whether to move based on the mouse's velocity
	currentMousePosition: The current mouse position
	previousMousePosition: The previous mouse position

	window;
*/

class Game
{
public:
	// Constructors
	Game(sf::RenderWindow&, int, int, sf::Texture&, Menu);
	Game(sf::RenderWindow&, Board, Menu);
	void setupBoard(int, int);
	void setupBoard();

	// Handlers
	void handleFrame();
	void handleEvent(const sf::Event&);
	void handlePlaying(const sf::Event&);

private:
	// Constant multipliers
	const float M_GLOBAL_MOVE = 1.5f;
	const float M_MOUSE_MOVE = 1.0f;
	const float M_KEYBOARD_MOVE = 10.0f;

	// Textures
	sf::Texture tileSet;

	// Game State
	Board board;
	GameState gameState;
	Menu menu;

	// Input State
	sf::Vector2f keyboardMove;
	sf::Vector2f mouseMove;

	bool doMouseMove;
	sf::Vector2f currentMousePosition;
	sf::Vector2f previousMousePosition;

	sf::RenderWindow& window;
};