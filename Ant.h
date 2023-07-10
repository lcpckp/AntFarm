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
	void LayTrail(PheromoneGrid& pheroGrid);
	float CalculatePheromoneFollowAngle(PheromoneGrid& pheroGrid);
	float getRandomAngle();
	void MovementTick(float deltaTime, PheromoneGrid& pheroGrid);
	float CalculatePheromoneFollowAngle_NEW_TEST(PheromoneGrid& pheroGrid);

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
	int forwardSampleDistance;
	int sideSampleDistance;
	float followStrength;
	float turnSpeed;

	// Dev Properties
	sf::VertexArray antSampleVision;
	bool visionOptions;
	

	// Predeclared temp variables
	int tileX, tileY;
	float distanceToFood, distanceToHome;
	float distanceX, distanceY;
	float directionX, directionY;
	float deltaX, deltaY;
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

