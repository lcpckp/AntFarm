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
	
private:
	// Utility
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	Resources resourceContainer;

	// Properties
	int foodAmount;
	sf::CircleShape body;


};

