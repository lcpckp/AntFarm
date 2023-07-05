#include "FoodSource.h"

std::pair<float, float> FoodSource::getPosition()
{
	return std::pair<float, float>(body.getPosition().x, body.getPosition().y);
}

bool FoodSource::takeFood()
{
	if (foodAmount > 0)
	{
		foodAmount--;
		return true;
	}

	return false;
}

void FoodSource::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(body);
}

FoodSource::FoodSource()
{
	
}

FoodSource::FoodSource(float x, float y)
{
	foodAmount = 100;
	Resources resourceContainer = Resources();
	body.setOrigin(10,10);
	body.setPosition(sf::Vector2f(x, y));
	body.setFillColor(resourceContainer.foodColor);
	body.setRadius(10.0f);
}