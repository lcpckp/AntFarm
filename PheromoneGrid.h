#pragma once
#include<SFML/Graphics.hpp>
#include"Resources.h"

enum pheroType { NONE, TO_FOOD, TO_HOME, TO_ENEMY };

class PheromoneGrid : public sf::Drawable
{
public:
	PheromoneGrid();
	~PheromoneGrid();
	void update(float deltaTime);

	// Accessors
	int getWidth();
	int getHeight();
	int getResolution(); // returns the size of the cells in pixels
	int getSize(); // returns number of individual cells in the grid

	float getHomeIntensity(int i);
	float getFoodIntensity(int i);
	float getIntensity(pheroType type, int i);
	float getIntensity(pheroType type, int x, int y);

	// Public functions
	void layTrail(int x, int y, pheroType type, float intensity);
	

	
private:
	// Utility
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
	// Properties
	int height, width, numCells, pheroResolution;
	float maxPheroPerCell;
	float decayRate;

	// Internal functions
	void updateHomeQuad(int i);
	void updateFoodQuad(int i);

	// Grid Data
	std::vector<float> toHomeIntensity; // intensity / alpha level of pheromone
	std::vector<float> toFoodIntensity;
	sf::VertexArray toHomeGridDrawable; // drawable object for the whole grid, contains quads of vertices
	sf::VertexArray toFoodGridDrawable;

	// Predeclared temp variables
	sf::Color quadColor;
	sf::Color newColor;
	int quadIndex;
	int gridIndex;
	int cellX, cellY;
};

