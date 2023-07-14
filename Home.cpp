#include "Home.h"
#include<iostream>

Home::Home(int x, int y)
{
	Resources resourceContainer = Resources();
	foodStoreCount = 0;
	body.setOrigin(20, 20);
	body.setPosition(sf::Vector2f(x, y));
	body.setRadius(20.0f);
	body.setFillColor(resourceContainer.homeColor);

	foodStoreText.setPosition(sf::Vector2f(x, y));
	foodStoreText.setFillColor(sf::Color::White);
	foodStoreText.setOutlineColor(sf::Color::Black);
	foodStoreText.setOutlineThickness(2);
	foodStoreText.setString(std::to_string(foodStoreCount));
	foodStoreText.setFont(Resources::generalFont);
	foodStoreText.setCharacterSize(24);
	foodStoreText.setOrigin(0, 0);
}

Home::~Home()
{

}

void Home::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(body);
	target.draw(foodStoreText);
}

std::pair<float, float> Home::getPosition()
{
	return std::pair<float, float>(body.getPosition().x, body.getPosition().y);
}

bool Home::depositFood()
{
	foodStoreCount++;
	foodStoreText.setString(std::to_string(foodStoreCount));
	return true;
}
