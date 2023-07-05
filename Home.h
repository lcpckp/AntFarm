#pragma once
#include<SFML/Graphics.hpp>
#include"Resources.h"
class Home : public sf::Drawable
{
public:
	Home(int x, int y);
	~Home();

	std::pair<float, float> getPosition();

	bool depositFood();
private:
	// Utility
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	Resources resourceContainer;
	//Ant spawnAnt(std::vector<Ant> antList);

	// Properties
	int foodStoreCount;
	sf::CircleShape body;
};

