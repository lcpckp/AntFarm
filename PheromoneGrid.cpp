#pragma once
#include "PheromoneGrid.h"
#include<iostream>

PheromoneGrid::PheromoneGrid()
{
	// Get dimensions from resource container
	height = Resources::farmHeight;
	width = Resources::farmWidth;
	numCells = height * width;
	pheroResolution = Resources::pheroResolution;

	// Pherogrid Settings
	maxPheroPerCell = 255;
	decayRate = 0.1f;

	// Instantiate grid data + grid drawable
	toHomeGridDrawable = sf::VertexArray(sf::PrimitiveType::Quads, numCells * 4);
	toFoodGridDrawable = sf::VertexArray(sf::PrimitiveType::Quads, numCells * 4);
	quadColor = Resources::groundColor;
	
	int x, y, quadIndex; // temp for upcoming quad index calculations
	

	// Loop through every cell and initialize vertex arrays
	for (int i = 0; i < numCells; i++)
	{
		// Fill each grid cell with empty cells
		toHomeIntensity.push_back(0.0f);
		toFoodIntensity.push_back(0.0f);

		// Calculate tile position in x, y from the width + index, then calculate the quad index for use in the vertex array
		x = i % width;
		y = i / width;
		quadIndex = i * 4;

		// set toHome Grid vertex positions
		toHomeGridDrawable[quadIndex].position = sf::Vector2f(x * pheroResolution, y * pheroResolution);
		toHomeGridDrawable[quadIndex + 1].position = sf::Vector2f((x + 1) * pheroResolution, y * pheroResolution);
		toHomeGridDrawable[quadIndex + 2].position = sf::Vector2f((x + 1) * pheroResolution, (y + 1) * pheroResolution);
		toHomeGridDrawable[quadIndex + 3].position = sf::Vector2f(x * pheroResolution, (y + 1) * pheroResolution);
		// set toHome color
		toHomeGridDrawable[quadIndex].color = Resources::homeTrailColor;
		toHomeGridDrawable[quadIndex + 1].color = Resources::homeTrailColor;
		toHomeGridDrawable[quadIndex + 2].color = Resources::homeTrailColor;
		toHomeGridDrawable[quadIndex + 3].color = Resources::homeTrailColor;

		// set toFood Grid vertex positions
		toFoodGridDrawable[quadIndex].position = sf::Vector2f(x * pheroResolution, y * pheroResolution);
		toFoodGridDrawable[quadIndex + 1].position = sf::Vector2f((x + 1) * pheroResolution, y * pheroResolution);
		toFoodGridDrawable[quadIndex + 2].position = sf::Vector2f((x + 1) * pheroResolution, (y + 1) * pheroResolution);
		toFoodGridDrawable[quadIndex + 3].position = sf::Vector2f(x * pheroResolution, (y + 1) * pheroResolution);
		// set toFood color
		toFoodGridDrawable[quadIndex].color = Resources::foodTrailColor;
		toFoodGridDrawable[quadIndex + 1].color = Resources::foodTrailColor;
		toFoodGridDrawable[quadIndex + 2].color = Resources::foodTrailColor;
		toFoodGridDrawable[quadIndex + 3].color = Resources::foodTrailColor;
	}
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

void PheromoneGrid::layTrail(int x, int y, pheroType type, float intensity)
{
	gridIndex = y * width + x;

	switch (type)
	{
	case pheroType::TO_HOME:
		toHomeIntensity[gridIndex] += intensity;
		if (toHomeIntensity[gridIndex] > maxPheroPerCell)
			toHomeIntensity[gridIndex] = maxPheroPerCell;
		break;
	case pheroType::TO_FOOD:
		toFoodIntensity[gridIndex] += intensity;
		if (toFoodIntensity[gridIndex] > maxPheroPerCell)
			toFoodIntensity[gridIndex] = maxPheroPerCell;
		break;
	case pheroType::TO_ENEMY:
		break;
	default:
		break;
	}
}

void PheromoneGrid::updateHomeQuad(int i)
{
	int j = i * 4;
	toHomeGridDrawable[j].color.a = toHomeIntensity[i];
	toHomeGridDrawable[j + 1].color.a = toHomeIntensity[i];
	toHomeGridDrawable[j + 2].color.a = toHomeIntensity[i];
	toHomeGridDrawable[j + 3].color.a = toHomeIntensity[i];
}

void PheromoneGrid::updateFoodQuad(int i)
{
	int j = i * 4;
	toFoodGridDrawable[j].color.a = toFoodIntensity[i];
	toFoodGridDrawable[j + 1].color.a = toFoodIntensity[i];
	toFoodGridDrawable[j + 2].color.a = toFoodIntensity[i];
	toFoodGridDrawable[j + 3].color.a = toFoodIntensity[i];
}

void PheromoneGrid::update(float deltaTime)
{
	// Run for each cell
	for (int i = 0; i < numCells; i++) 
	{
		// If there is some level of pheromone present
		if (toHomeIntensity[i] > 0 || toFoodIntensity[i] > 0) 
		{
			// Decay food and home pheromones by the decay rate per second
			toFoodIntensity[i] *= std::pow((1 - decayRate), deltaTime); 
			toHomeIntensity[i] *= std::pow((1 - decayRate), deltaTime);

			// If the pheromone level is less than 0.25, snap the value down to 0.0
			if (toHomeIntensity[i] + toFoodIntensity[i] < 0.25f) 
			{
				toHomeIntensity[i] = 0.0f;
				toFoodIntensity[i] = 0.0f;
			}
			
			// Update the drawable quads with the new intensity value
			updateHomeQuad(i);
			updateFoodQuad(i);
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
	// If out of bounds, return 0 intensity
	if (i < 0 || i > getSize())
	{
		return 0;
	}

	// Pick grid based on type, and return intensity
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
}

float PheromoneGrid::getIntensity(pheroType type, int x, int y)
{
	return getIntensity(type, y * width + x);
}

void PheromoneGrid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(toHomeGridDrawable);
	target.draw(toFoodGridDrawable);
}


	







