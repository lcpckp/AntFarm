#include "Resources.h"

sf::Font Resources::generalFont;

Resources::Resources()
{
	generalFont.loadFromFile("Lavigne.ttf");
}

Resources::~Resources()
{

}