#pragma once
#include<SFML/Graphics.hpp>



class Resources
{
public:
	Resources();
	~Resources();
	static const int farmWidth = 960;
	static const int farmHeight = 540; 
	static const int pheroResolution = 3;
	static const int spawnClick = 200;

	static float antBodyRadius;

	static sf::Font generalFont;

	static sf::Color groundColor;
	static sf::Color antColor;
	static sf::Color homeTrailColor;
	static sf::Color foodTrailColor;
	static sf::Color enemyTrailColor;
	static sf::Color homeColor;
	static sf::Color foodColor;
	static sf::Color enemyColor;
private:
	
};
 
