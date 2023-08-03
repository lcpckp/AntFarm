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
		foodAmountText.setString(std::to_string(foodAmount));
		return true;
	}

	return false;
}

bool FoodSource::hasFood()
{
	return foodAmount > 0;
}

void FoodSource::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(body);
	
	if (Resources::showInfo)
	{
		target.draw(foodAmountText);
	}
}

FoodSource::FoodSource()
{
	
}

FoodSource::FoodSource(float x, float y)
{
	foodAmount = Resources::foodStartingAmount;
	body.setOrigin(Resources::foodBodyRadius, Resources::foodBodyRadius);
	body.setPosition(sf::Vector2f(x, y));
	body.setFillColor(Resources::foodColor);
	body.setRadius(Resources::foodBodyRadius);

	foodAmountText.setPosition(sf::Vector2f(x, y));
	foodAmountText.setFillColor(sf::Color::White);
	foodAmountText.setOutlineColor(sf::Color::Black);
	foodAmountText.setOutlineThickness(2);
	foodAmountText.setString(std::to_string(foodAmount));
	foodAmountText.setFont(Resources::generalFont);
	foodAmountText.setCharacterSize(24);
	foodAmountText.setOrigin(10, 10);

}