#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

const int NUM_OPTIONS = 2;

enum class Option
{
	play,
	exit,
	none = -1
};

/*
	Menu
	-- Store menu textures
*/
class Menu
{
public:
	// Constructors
	Menu(std::vector<sf::Sprite>&, std::vector<sf::Sprite>&);

	// Option operations
	void nextOption(const Option&);

	// Handlers
	void handleEvent(const sf::Event&);

	// Screen operations
	void displayMenu(sf::RenderWindow&) const;

private:
	std::vector<sf::Sprite> decoration;
	std::vector<sf::Sprite> options;

	Option optionSelected;
};

