#include "Resources.h"

sf::Font Resources::generalFont;
sf::Color Resources::groundColor;
sf::Color Resources::antColor;
sf::Color Resources::homeTrailColor;
sf::Color Resources::foodTrailColor;
sf::Color Resources::enemyTrailColor;
sf::Color Resources::homeColor;
sf::Color Resources::foodColor;
sf::Color Resources::enemyColor;

float Resources::antBodyRadius;

Resources::Resources()
{
	generalFont.loadFromFile("Lavigne.ttf");
	groundColor = sf::Color(254, 217, 155);
	antColor = sf::Color(31, 32, 65);
	homeTrailColor = sf::Color(91, 133, 170, 0);
	foodTrailColor = sf::Color(171, 225, 136, 0);
	enemyTrailColor = sf::Color(226, 181, 146, 0);
	homeColor = sf::Color(81, 123, 160);
	foodColor = sf::Color(161, 215, 126);
	enemyColor = sf::Color(216, 171, 136);

	antBodyRadius = 2.0f;
}

Resources::~Resources()
{

}