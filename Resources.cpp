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
float Resources::homeBodyRadius;
float Resources::foodBodyRadius;

int Resources::pheroResolution;
float Resources::decayRate;
float Resources::maxPheroPerCell;
int Resources::farmHeight;
int Resources::farmWidth;

int Resources::maxFoodSources;
int Resources::foodStartingAmount;

int Resources::numAntsPerClick;
int Resources::numStartingAnts;


Resources::Resources()
{
	// Visuals
	generalFont.loadFromFile("res\\Lavigne.ttf");
	antBodyRadius = 2.0f;
	homeBodyRadius = 20.0f;
	foodBodyRadius = 10.0f;

	// Global settings
	farmWidth = 960;
	farmHeight = 540;

	// Pheromone Grid settings
	pheroResolution = 3;
	decayRate = 0.05f;
	maxPheroPerCell = 1000.0f;

	// Simulation Rules
	maxFoodSources = 6;
	numAntsPerClick = 5;

	// Starting Conditions
	foodStartingAmount = 99999;
	numStartingAnts = 500;

	// Color Scheme
	groundColor = sf::Color(254, 217, 155);
	antColor = sf::Color(31, 32, 65);
	homeTrailColor = sf::Color(91, 133, 170, 0);
	foodTrailColor = sf::Color(171, 225, 136, 0);
	enemyTrailColor = sf::Color(226, 181, 146, 0);
	homeColor = sf::Color(81, 123, 160);
	foodColor = sf::Color(161, 215, 126);
	enemyColor = sf::Color(216, 171, 136);
}

Resources::~Resources()
{

}