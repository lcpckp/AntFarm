#pragma once
#include<cmath>
#include<iostream>
#include<vector>
#include "Ant.h"
#include "FoodSource.h"
#include "PheromoneGrid.h"


Ant::Ant(int x, int y, PheromoneGrid& pheroGrid)
{
	// Ant properties
	body.setOrigin(1, 1);
	body.setPosition(x, y);
	body.setRadius(1);
	body.setFillColor(resourceContainer.antColor);
	hasFood = false;

	// Movement Settings
	movementSpeed = 50.0f;
	movementHeading = (std::rand() / (RAND_MAX + 1.0f)) * 2 * 4 - 4;
	movementRandomness = 0.001f;

	// Thresholds
	touchThreshold = 10.0f;
	sightThreshold = 50.0f;

	// Trail laying/following settings
	fallOffMultiplier = 2.0f; // How quickly their trail strength dies off as they move away from home or food
	maxPheroStrength = 25.0f; // How much pheromone to lay down per tick
	forwardSampleDistance = 10;
	sideSampleDistance = 10;
	followStrength = 0.1f; // How much to obey the pheromones

	// Help/Reference
	gridWidth = pheroGrid.getWidth();
	gridHeight = pheroGrid.getHeight();
	gridSize = pheroGrid.getSize();

	// Debug stuff
	antSampleVision = sf::VertexArray(sf::Lines, 2);
	visionOptions = true;
	
	// Preallocated temp variables (for speed I guess? I don't actually know)
	timeSinceHome = 0.0f;
	timeSinceFood = 0.0f;
	tileX = 0, tileY = 0;
	distanceToFood = 1000.0f;
	distanceToHome = 1000.0f;
	distanceX = 0, distanceY = 0.0f;
	directionX = 0, directionY = 0.0f;
	deltaX = 0, deltaY = 0.0f;
	sampleCenterX = 0, sampleCenterY = 0;
	currCellX = 0, currCellY = 0;
	seekingTrailType = pheroType::TO_FOOD;
	totalStrength = 0.0f;
	maxStrength = 0.0f;
	index = 0;
	turningAngle = 0.0f;

	
}

Ant::~Ant()
{

}

void Ant::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (visionOptions)
	{
		target.draw(antSampleVision);
	}
	
	target.draw(body);
}

void Ant::updateAnt(PheromoneGrid& pheroGrid, std::vector<FoodSource>& foodList, std::vector<Home>& homeList, float deltaTime)
{
	// Add elapsed time to timeSinceHome/Food counters
	timeSinceHome += deltaTime;
	timeSinceFood += deltaTime; //might be able to just have one but will need two if they can do two trails at once

	// Check if we can grab or see a food source
	// Also updates heading directly to food source if within sight threshold
	if (!hasFood)
		TryGetFood(foodList);
	else
		TryDepositFood(homeList);
	
	
	
	// Sample pheromones and update heading accordingly
	desiredHeading = CalculatePheromoneFollowAngle(pheroGrid);

	float headingDifference = (movementHeading - desiredHeading) * followStrength * deltaTime;
	movementHeading += headingDifference;
	
	// Lay a trail
	LayTrail(pheroGrid);

	// Adjust heading by small random amount
	movementHeading += getRandomAngle();

	// Process the movement of the ant
	MovementTick(deltaTime, pheroGrid);
	
}

void Ant::MovementTick(float deltaTime, PheromoneGrid& pheroGrid)
{
	// Get vector direction using trigonometry on the heading variable, then calculate movement vector with speed variable.
	sf::Vector2f direction(std::cos(movementHeading), std::sin(movementHeading));
	sf::Vector2f movement = direction * movementSpeed;

	// Update body of any by the movement vector * deltaTime
	body.setPosition(body.getPosition() + (movement * deltaTime));

	// Handle out of bounds ants (Reverse heading + recalculate new position)
	if (body.getPosition().x > pheroGrid.getWidth() || body.getPosition().y > pheroGrid.getHeight() || body.getPosition().x < 0 || body.getPosition().y < 0)
	{
		movementHeading += 3.14f;
		body.setPosition(sf::Vector2f((float)std::max(0, std::min((int)body.getPosition().x, gridWidth)), (float)std::max(0, std::min((int)body.getPosition().y, gridHeight))));
	}
}

float Ant::getRandomAngle()
{
	return (std::rand() / (RAND_MAX + 1.0f)) * 2 * movementRandomness - movementRandomness;
}

void Ant::TryDepositFood(std::vector<Home>& homeList)
{
	for (int i = 0; i < homeList.size(); i++)
	{
		distanceToHome = std::sqrt(std::pow(body.getPosition().x - homeList[i].getPosition().first, 2) + std::pow(body.getPosition().y - homeList[i].getPosition().second, 2));

		if (distanceToHome < touchThreshold)
		{
			if (homeList[i].depositFood())
			{
				hasFood = false;
				timeSinceHome = 0.0f;
				seekingTrailType = pheroType::TO_FOOD;
				movementHeading += 3.14f;
			}
		}
		else if (distanceToHome < sightThreshold)
		{
			distanceX = homeList[i].getPosition().first - body.getPosition().x;
			distanceY = homeList[i].getPosition().second - body.getPosition().y;
			movementHeading = std::atan2(distanceY, distanceX);
		}
	}
}

void Ant::TryGetFood(std::vector<FoodSource>& foodList)
{
	
	for (int i = 0; i < foodList.size(); i++)
	{
		distanceToFood = std::sqrt(std::pow(body.getPosition().x - foodList[i].getPosition().first, 2) + std::pow(body.getPosition().y - foodList[i].getPosition().second, 2));

		if (distanceToFood < touchThreshold)
		{
			if (foodList[i].takeFood())
			{
				hasFood = true;
				timeSinceFood = 0.0f;
				seekingTrailType = pheroType::TO_HOME;
				movementHeading += 3.14f;
			}
		}
		else if (distanceToFood < sightThreshold)
		{
			distanceX = foodList[i].getPosition().first - body.getPosition().x;
			distanceY = foodList[i].getPosition().second - body.getPosition().y;
			movementHeading = std::atan2(distanceY, distanceX);
		}
	}
	
}

/*
		Lay trail based on ant's goals (calculates tile x and y based on body's position x and y)

		-If ant has food, lay down "to food" trails. Intensity is based on how long it's been since we've seen the food source, with a minimum of 0.
		-If the ant has recently been home, lay down "to home" trails. This also falls off based on how long it's been since the ant was home.
	*/
void Ant::LayTrail(PheromoneGrid& pheroGrid)
{
	if (hasFood)
	{
		pheroGrid.layTrail(body.getPosition().x / pheroGrid.getResolution(), body.getPosition().y / pheroGrid.getResolution(), pheroType::TO_FOOD, std::max(maxPheroStrength - (timeSinceFood * fallOffMultiplier), 0.0f));
	}
	else
	{
		pheroGrid.layTrail(body.getPosition().x / pheroGrid.getResolution(), body.getPosition().y / pheroGrid.getResolution(), pheroType::TO_HOME, std::max(maxPheroStrength - (timeSinceHome * fallOffMultiplier), 0.0f));
	}
}

float Ant::CalculatePheromoneFollowAngle(PheromoneGrid& pheroGrid)
{
	float highestStrength = 0.0;
	int maxCellX = 0;
	int maxCellY = 0;

	// Get the cell that the ant is standing on
	sampleCenterX = body.getPosition().x / resourceContainer.pheroResolution;
	sampleCenterY = body.getPosition().y / resourceContainer.pheroResolution;

	// Loop through sampling distance to get offsets
	for (int x = -sideSampleDistance; x <= sideSampleDistance; x++)
	{
		for (int y = -forwardSampleDistance; y <= forwardSampleDistance; y++)
		{
			if (!(x == 0 && y == 0))
			{
				// Get current cell with offsets
				currCellX = sampleCenterX + x;
				currCellY = sampleCenterY + y;

				// Calculate grid index based on x, y
				index = currCellY * gridWidth + currCellX;

				// If in bounds
				if (index > 0 && index < gridSize)
				{
					// If this cell is the strongest, update the highestStrength value + direction of this cell.
					if (pheroGrid.getIntensity(seekingTrailType, index) > highestStrength)
					{
						highestStrength = pheroGrid.getIntensity(seekingTrailType, index);
						maxCellX = currCellX;
						maxCellY = currCellY;
					}
				}
			}
		}
	} // End sampling loop

	directionX = maxCellX - sampleCenterX;
	directionY = maxCellY - sampleCenterY;

	if (highestStrength <= 5)
		return movementHeading;



	float averageHeading = std::atan2(directionY, directionX);

	antSampleVision[0] = body.getPosition();
	antSampleVision[1] = sf::Vector2f(body.getPosition().x + 10 * std::cos(averageHeading), body.getPosition().y + 10 * std::sin(averageHeading));

	return averageHeading;
}


