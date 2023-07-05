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
}

Home::~Home()
{

}

void Home::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(body);
}

std::pair<float, float> Home::getPosition()
{
	return std::pair<float, float>(body.getPosition().x, body.getPosition().y);
}

bool Home::depositFood()
{
	foodStoreCount++;
	return true;
}
