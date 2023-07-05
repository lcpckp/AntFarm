#pragma once
#include "PheromoneGrid.h"
#include<iostream>

PheromoneGrid::PheromoneGrid()
{
	// Get resources container
	Resources resourceContainer = Resources();
	// Get dimensions from resource container
	height = resourceContainer.farmHeight;
	width = resourceContainer.farmWidth;
	numCells = height * width;
	pheroResolution = resourceContainer.pheroResolution;

	quadColor = resourceContainer.groundColor;

	// Instantiate grid data + grid drawable
	toHomeGridDrawable = sf::VertexArray(sf::PrimitiveType::Quads, numCells * 4);
	toFoodGridDrawable = sf::VertexArray(sf::PrimitiveType::Quads, numCells * 4);

	decayRate = 0.05f;

	int x, y; // temp for upcoming quad index calculations

	for (int i = 0; i < numCells; i++)
	{
		toHomeIntensity.push_back(0.0f);
		toFoodIntensity.push_back(0.0f);

		// Calculate tile position in x, y from the width + index, then calculate the quad index for use in the vertex array
		x = i % width;
		y = i / width;
		int quadIndex = i * 4;

		// toHome Grid positions
		toHomeGridDrawable[quadIndex].position = sf::Vector2f(x * pheroResolution, y * pheroResolution);
		toHomeGridDrawable[quadIndex + 1].position = sf::Vector2f((x + 1) * pheroResolution, y * pheroResolution);
		toHomeGridDrawable[quadIndex + 2].position = sf::Vector2f((x + 1) * pheroResolution, (y + 1) * pheroResolution);
		toHomeGridDrawable[quadIndex + 3].position = sf::Vector2f(x * pheroResolution, (y + 1) * pheroResolution);
		// toHome color
		toHomeGridDrawable[quadIndex].color = resourceContainer.homeTrailColor;
		toHomeGridDrawable[quadIndex + 1].color = resourceContainer.homeTrailColor;
		toHomeGridDrawable[quadIndex + 2].color = resourceContainer.homeTrailColor;
		toHomeGridDrawable[quadIndex + 3].color = resourceContainer.homeTrailColor;

		// toFood Grid positions
		toFoodGridDrawable[quadIndex].position = sf::Vector2f(x * pheroResolution, y * pheroResolution);
		toFoodGridDrawable[quadIndex + 1].position = sf::Vector2f((x + 1) * pheroResolution, y * pheroResolution);
		toFoodGridDrawable[quadIndex + 2].position = sf::Vector2f((x + 1) * pheroResolution, (y + 1) * pheroResolution);
		toFoodGridDrawable[quadIndex + 3].position = sf::Vector2f(x * pheroResolution, (y + 1) * pheroResolution);
		// toFood color
		toFoodGridDrawable[quadIndex].color = resourceContainer.foodTrailColor;
		toFoodGridDrawable[quadIndex + 1].color = resourceContainer.foodTrailColor;
		toFoodGridDrawable[quadIndex + 2].color = resourceContainer.foodTrailColor;
		toFoodGridDrawable[quadIndex + 3].color = resourceContainer.foodTrailColor;
	}

	std::cout << "[DEBUG] Empty Pheromone Grid Initialized" << std::endl;
}

PheromoneGrid::~PheromoneGrid()
{
	
}

int PheromoneGrid::getResolution()
{
	return pheroResolution;
}

int PheromoneGrid::getWidth()
{
	return width;
}

int PheromoneGrid::getHeight()
{
	return height;
}

/*
	layTrail(int x, int y, pheroType type)

	-Calculates the quadIndex to access the tile at x, y's vertices.
	-Picks the correct color based on type parameter.
	-Updates the quads color + intensity (calculates the correct index for the pheroGrid vector using x and y)
	-Intensity can be 0-255
*/

void PheromoneGrid::layTrail(int x, int y, pheroType type, float intensity)
{
	gridIndex = y * width + x;
	
	switch (type)
	{
	case pheroType::TO_HOME:
		toHomeIntensity[gridIndex] += intensity;
		if (toHomeIntensity[gridIndex] > 255)
			toHomeIntensity[gridIndex] = 255;
		break;
	case pheroType::TO_FOOD:
		toFoodIntensity[gridIndex] += intensity;
		if (toFoodIntensity[gridIndex] > 255)
			toFoodIntensity[gridIndex] = 255;
		break;
	case pheroType::TO_ENEMY:
		break;
	default:
		break;
	}

	

	
	

}

/*
	update(float deltaTime)

	-Loops through every cell, if no pheromones to update then it simply moves on to the next cell.
	-On cells with pheromones, it decays by some rate, snapping to 0 if under 1/255 intensity.
	-Then it finds the quadIndex for the cell based on the index, and updates the color's alpha to the new intensity value
*/

void PheromoneGrid::update(float deltaTime)
{
	for (int i = 0; i < numCells; i++)
	{
		if (toHomeIntensity[i] > 0 || toFoodIntensity[i] > 0)
		{
			toFoodIntensity[i] *= (1.0f - decayRate * deltaTime);
			toHomeIntensity[i] *= (1.0f - decayRate * deltaTime);

			if (toHomeIntensity[i] + toFoodIntensity[i] < 1.0f)
			{
				toHomeIntensity[i] = 0;
				toFoodIntensity[i] = 0;
			}
			
			cellX = i % width;
			cellY = i / width;
			
			quadIndex = (cellX + cellY * width) * 4;

			toHomeGridDrawable[quadIndex].color.a = toHomeIntensity[i];
			toHomeGridDrawable[quadIndex + 1].color.a = toHomeIntensity[i];
			toHomeGridDrawable[quadIndex + 2].color.a = toHomeIntensity[i];
			toHomeGridDrawable[quadIndex + 3].color.a = toHomeIntensity[i];

			toFoodGridDrawable[quadIndex].color.a = toFoodIntensity[i];
			toFoodGridDrawable[quadIndex + 1].color.a = toFoodIntensity[i];
			toFoodGridDrawable[quadIndex + 2].color.a = toFoodIntensity[i];
			toFoodGridDrawable[quadIndex + 3].color.a = toFoodIntensity[i];
		}
	}
}

int PheromoneGrid::getSize()
{
	return toHomeIntensity.size();
}

float PheromoneGrid::getHomeIntensity(int i)
{
	return toHomeIntensity[i];
}
float PheromoneGrid::getFoodIntensity(int i)
{
	return toFoodIntensity[i];
}
float PheromoneGrid::getIntensity(pheroType type, int i)
{
	switch (type)
	{
	case pheroType::TO_HOME:
		return getHomeIntensity(i);
	case pheroType::TO_FOOD:
		return getFoodIntensity(i);
	case pheroType::TO_ENEMY:
		return 0; //not implemented
	default:
		return 0;
	}
	return toHomeIntensity[i];
}

void PheromoneGrid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(toHomeGridDrawable);
	target.draw(toFoodGridDrawable);
}


	







