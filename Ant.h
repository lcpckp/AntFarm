#pragma once
#include<SFML/Graphics.hpp>
#include"PheromoneGrid.h"
#include"FoodSource.h"
#include"Home.h"

class Ant : public sf::Drawable
{
public:
	Ant(int x, int y, PheromoneGrid& pheroGrid);
	~Ant();
	void updateAnt(PheromoneGrid &pheroGrid, std::vector<FoodSource>& foodList, std::vector<Home>& homeList, float deltaTime);
	float getLifetime();

private:
	// Utility
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// Functions
	void TryGetFood(std::vector<FoodSource>& foodList);
	void TryDepositFood(std::vector<Home>& homeList);
	void LayTrail(PheromoneGrid& pheroGrid, float deltaTime);
	float CalculatePheromoneFollowAngle(PheromoneGrid& pheroGrid);
	float getRandomAngle();
	void MovementTick(float deltaTime, PheromoneGrid& pheroGrid);

	// Properties
	sf::CircleShape body; // canonical x, y position
	float movementSpeed;
	float movementHeading;
	float desiredHeading;
	float movementRandomness;
	float timeSinceHome;
	float timeSinceFood;
	float lifetime;
	float touchThreshold;
	float sightThreshold;
	float fallOffMultiplier;
	float maxPheroStrength;
	bool hasFood;
	int sampleTurnAngle;
	int sampleCount;
	float maxSampleDistance;
	float sampleIgnoreThreshold;
	float followStrength;
	float turnSpeed;
	pheroType seekingTrailType;
	

	int gridWidth, gridHeight;
	int gridSize;

	// Dev Properties
	sf::VertexArray antSampleVision;
	bool visionOptions;

	// I'm having to declare these here for some reason even though they are only used in temporary calculations later
	int currCellX, currCellY;
};

