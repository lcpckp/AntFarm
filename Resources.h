#pragma once
#include<SFML/Graphics.hpp>

class Resources
{
public:
	Resources();
	~Resources();
	const int farmWidth = 1400;
	const int farmHeight = 900;
	const int pheroResolution = 5;
	const int spawnClick = 50;

	const sf::Color groundColor = sf::Color(254, 217, 155);
	const sf::Color antColor = sf::Color(31, 32, 65);
	const sf::Color homeTrailColor = sf::Color(91, 133, 170, 0);
	const sf::Color foodTrailColor = sf::Color(171, 225, 136, 0);
	const sf::Color enemyTrailColor = sf::Color(226, 181, 146, 0);
	const sf::Color homeColor = sf::Color(81, 123, 160);
	const sf::Color foodColor = sf::Color(161, 215, 126);
	const sf::Color enemyColor = sf::Color(216, 171, 136);
private:
	
};
 
