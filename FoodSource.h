#pragma once
#include<SFML\Graphics.hpp>
#include"Resources.h"

class FoodSource : public sf::Drawable
{
public:
	FoodSource();
	FoodSource(float x, float y);
	
	std::pair<float, float> getPosition();
	bool takeFood();
	bool hasFood();
	
private:
	// Utility
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// Properties
	int foodAmount;
	sf::CircleShape body;
	sf::Text foodAmountText;


};

