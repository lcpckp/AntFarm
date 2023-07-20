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
	body.setOrigin(Resources::antBodyRadius, Resources::antBodyRadius);
	body.setPosition(x, y);
	body.setRadius(Resources::antBodyRadius);
	body.setFillColor(Resources::antColor);
	hasFood = false;

	// Movement Settings
	movementSpeed = 45.0f;
	movementHeading = (std::rand() / (RAND_MAX + 1.0f)) * 2 * 4 - 4;
	movementRandomness = 0.125f;
	touchThreshold = 5.0f;
	sightThreshold = 50.0f;

	// Pheromone Sampling
	sampleTurnAngle = 3;
	sampleCount = 15;
	maxSampleDistance = 15.0f;
	sampleIgnoreThreshold = 1.0f;

	// Trail laying
	fallOffMultiplier = 5.0f; // How quickly their trail strength dies off as they move away from home or food
	maxPheroStrength = 200.0f; // How much pheromone to lay down per tick
	followStrength = 5.0f; // How much to obey the pheromones
	seekingTrailType = pheroType::TO_FOOD; // default is looking for food

	// Help/Reference
	gridWidth = pheroGrid.getWidth();
	gridHeight = pheroGrid.getHeight();
	gridSize = pheroGrid.getSize();

	// Debug stuff
	antSampleVision = sf::VertexArray(sf::Lines, 2);
	visionOptions = false;
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
	
	
	
	// Sample and calculate desired heading, and difference between desired + actual heading
	desiredHeading = CalculatePheromoneFollowAngle(pheroGrid);
	float headingDifference = (movementHeading - desiredHeading) * followStrength * deltaTime;

	// Normalize current heading before adding heading difference

	movementHeading = fmod(movementHeading, (2.0f * 3.14f));
	movementHeading -= headingDifference;
	
	// Lay a trail
	LayTrail(pheroGrid, deltaTime);

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
	if (body.getPosition().x > gridWidth || body.getPosition().y > gridHeight || body.getPosition().x < 0 || body.getPosition().y < 0)
	{
		movementHeading += 3.14f; // Turn around
		timeSinceHome += 5.0f; // Penalty to pheromones for hitting a wall
		timeSinceFood += 5.0f; // Penalty to pheromones for hitting a wall
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
		float distanceToHome = std::sqrt(std::pow(body.getPosition().x - homeList[i].getPosition().first, 2) + std::pow(body.getPosition().y - homeList[i].getPosition().second, 2));

		if (distanceToHome < touchThreshold)
		{
			if (homeList[i].depositFood())
			{
				hasFood = false;
				timeSinceHome = 0.0f;
				seekingTrailType = pheroType::TO_FOOD;
				movementHeading += 3.14;
			}
		}
		else if (distanceToHome < sightThreshold)
		{
			float distanceX = homeList[i].getPosition().first - body.getPosition().x;
			float distanceY = homeList[i].getPosition().second - body.getPosition().y;
			movementHeading = std::atan2(distanceY, distanceX);
		}
	}
}

void Ant::TryGetFood(std::vector<FoodSource>& foodList)
{
	
	for (int i = 0; i < foodList.size(); i++)
	{
		float distanceToFood = std::sqrt(std::pow(body.getPosition().x - foodList[i].getPosition().first, 2) + std::pow(body.getPosition().y - foodList[i].getPosition().second, 2));

		if (distanceToFood < touchThreshold)
		{
			if (foodList[i].takeFood())
			{
				hasFood = true;
				timeSinceFood = 0.0f;
				seekingTrailType = pheroType::TO_HOME;
				movementHeading += 3.14;
			}
		}
		else if (distanceToFood < sightThreshold && foodList[i].hasFood())
		{
			float distanceX = foodList[i].getPosition().first - body.getPosition().x;
			float distanceY = foodList[i].getPosition().second - body.getPosition().y;
			movementHeading = std::atan2(distanceY, distanceX);
		}
	}
	
}

void Ant::LayTrail(PheromoneGrid& pheroGrid, float deltaTime)
{
	// If ant has food, lay a "to Food" trail at decreasing strength based on the time it's been since it found food.
	if (hasFood)
	{
		pheroGrid.layTrail(body.getPosition().x / pheroGrid.getResolution(), body.getPosition().y / pheroGrid.getResolution(), pheroType::TO_FOOD, std::max((maxPheroStrength - (timeSinceFood * fallOffMultiplier)) * deltaTime, 0.0f));
	}
	// Otherwise, do the same thing with a "to Home" trail.
	else
	{
		pheroGrid.layTrail(body.getPosition().x / pheroGrid.getResolution(), body.getPosition().y / pheroGrid.getResolution(), pheroType::TO_HOME, std::max((maxPheroStrength - (timeSinceHome * fallOffMultiplier)) * deltaTime, 0.0f));
	}
}

float Ant::CalculatePheromoneFollowAngle(PheromoneGrid& pheroGrid)
{
	float bestSampleStrength = 0;
	int bestAngle = 0;

	// This sampling loop is based on two settings: sampleCount and sampleTurnAngle.
	// sampleCount is the number of sample raycasts to do per side (left and right)
	// sampleTurnAngle is the angle in degrees of each sample.
	// The loop begins at the farthest negative turn and ends at the farthest positive turn, incrementing by the sampleTurnAngle each time.

	for (int i = sampleCount * -sampleTurnAngle; i <= sampleTurnAngle * sampleCount; i += sampleTurnAngle)
	{
		// reset sampleDistance to max value (decreases to 0 as part of sampling)
		float sampleDistance = maxSampleDistance;

		// reset totalSampleStrength to 0 to accumulate current sample
		float totalSampleStrength = 0;

		// get actual sample angle by adding current sampleAngle to the current heading of the ant.
		float sampleOrientation = fmod((movementHeading + (i * 3.14 / 180.0)), (2.0 * 3.14));

		// calculate the x and y steps for sampling based on the current orientation
		float stepX = std::cos(sampleOrientation) * Resources::pheroResolution;
		float stepY = std::sin(sampleOrientation) * Resources::pheroResolution;

		// start currentX and currentY at the current position of the ant
		float currentX = body.getPosition().x;
		float currentY = body.getPosition().y;
		
		// Samples various points at decreasing distances until it reaches 0
		while (sampleDistance >= 0)
		{
			// get the X and Y of the current cell we are sampling
			int currCellX = std::round(currentX) / Resources::pheroResolution;
			int currCellY = std::round(currentY) / Resources::pheroResolution;
			
			// add the strength from the cell to the accumulator
			totalSampleStrength += pheroGrid.getIntensity(seekingTrailType, currCellX, currCellY);

			// increment the current position by the step values
			currentX += stepX;
			currentY += stepY;
			sampleDistance -= Resources::pheroResolution;
		}

		// if this sample is the best so far, save that information to the best variables
		if (totalSampleStrength > bestSampleStrength)
		{
			bestSampleStrength = totalSampleStrength;
			bestAngle = i;
		}
	}

	// if the best sample we found was too weak, ignore it.
	if (bestSampleStrength < sampleIgnoreThreshold)
	{
		return movementHeading;
	}
	// otherwise, return the bestAngle converted to radians, added to our current movement heading to give the correct desired heading.
	else
	{
		return fmod((movementHeading + (bestAngle * 3.14 / 180.0)), (2.0 * 3.14));
	}
}


