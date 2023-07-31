#pragma once
#include<SFML/Graphics.hpp>



class Resources
{
public:
	Resources();
	~Resources();
	static int farmWidth;
	static int farmHeight;

	static int pheroResolution;
	static float decayRate;
	static float maxPheroPerCell;

	static int numAntsPerClick;
	static int maxFoodSources;
	static int foodStartingAmount;

	static int numStartingAnts;

	static float antBodyRadius;
	static float homeBodyRadius;
	static float foodBodyRadius;

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
 
