#include "menu.h"

Menu::Menu(std::vector<sf::Sprite>& _decoration, std::vector<sf::Sprite>& _options) :
	decoration(_decoration), options(_options)
{
}

// Changes options within range
void Menu::nextOption(const Option& option)
{

}

// Handles event
void Menu::handleEvent(const sf::Event& event)
{
    switch (event.type)
    {
        // Keyboard
    case sf::Event::KeyPressed:
        switch (event.key.code)
        {
        case sf::Keyboard::W:
            break;
        case sf::Keyboard::S:
            break;
        default:
            break;
        }
        break;

    case sf::Event::KeyReleased:
        switch (event.key.code)
        {
        case sf::Keyboard::Enter:
            break;
        default:
            break;
        }
        break;

        // Mouse
    case sf::Event::MouseButtonPressed:
        if (event.mouseButton.button == sf::Mouse::Left)
        {

        }
        break;

    case sf::Event::MouseButtonReleased:
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            
        }
        break;

    default:
        break;
    }
}

// Displays all decoration and options sprites
void Menu::displayMenu(sf::RenderWindow& window) const
{
	// TODO: Figure out a way to do both in one
	for (const auto& s : this->decoration)
		window.draw(s);
	for (const auto& s : this->options)
		window.draw(s);
}