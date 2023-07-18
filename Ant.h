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

private:
	// Utility
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	Resources resourceContainer;

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
	float touchThreshold;
	float sightThreshold;
	float fallOffMultiplier;
	float maxPheroStrength;
	bool hasFood;
	int sampleTurnAngle;
	int numSamples;
	float maxSampleDistance;
	float sampleIgnoreThreshold;
	float followStrength;
	float turnSpeed;

	// Dev Properties
	sf::VertexArray antSampleVision;
	bool visionOptions;
	

	// Predeclared temp variables
	float distanceToFood, distanceToHome;
	float distanceX, distanceY;
	float directionX, directionY;
	int sampleCenterX, sampleCenterY;
	int currCellX, currCellY;
	pheroType seekingTrailType;
	float totalStrength;
	float maxStrength;
	int index;
	float turningAngle;
	int gridWidth, gridHeight;
	int gridSize;
};

