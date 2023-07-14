#include<iostream>
#include<SFML/Graphics.hpp>
#include"Home.h"
#include"Ant.h"
#include"Resources.h"
#include"PheromoneGrid.h"
#include"FoodSource.h"


bool drawTrails = true;
void handleInput(sf::RenderWindow &window, std::vector<Ant> &antList, std::vector<FoodSource>& foodList, PheromoneGrid& pheroGrid, Resources& resourceContainer);
void updateObjects(std::vector<Ant> &antList, PheromoneGrid &pheroGrid, std::vector<FoodSource>& foodList, std::vector<Home>& homeList, float deltaTime);
void drawObjects(sf::RenderWindow &window, std::vector<Ant> &antList, PheromoneGrid &pheroGrid, std::vector<FoodSource>& foodList, std::vector<Home>& homeList);

int main()
{
    // Set up resource container (contains general settings + textures), render window from SFML, and clock for deltaTime for framerate independence
    Resources resourceContainer = Resources();
    sf::RenderWindow window(sf::VideoMode(resourceContainer.farmWidth, resourceContainer.farmHeight), "Ant Farm");
    window.setFramerateLimit(120);
    sf::Clock clock;
    float deltaTime = 0.0f;

    // Game objects
    std::vector<Ant> antList;
    PheromoneGrid pheroGrid;
    std::vector<FoodSource> foodList;
    std::vector<Home> homeList;

    
    // Temporary start conditions
    homeList.push_back(Home(230, 150));
    homeList.push_back(Home(800, 400));

    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
        // Handle Inputs
        handleInput(window, antList, foodList, pheroGrid, resourceContainer);

        // Update objects
        updateObjects(antList, pheroGrid, foodList, homeList, deltaTime);

        // Draw objects
        drawObjects(window, antList, pheroGrid, foodList, homeList);
    }

	return 0;
}

void handleInput(sf::RenderWindow &window, std::vector<Ant> &antList, std::vector<FoodSource>& foodList, PheromoneGrid& pheroGrid, Resources& resourceContainer)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
            {
                window.close();
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    for (int i = 0; i < resourceContainer.spawnClick; i++)
                    {
                        Ant newAnt = Ant(event.mouseButton.x, event.mouseButton.y, pheroGrid);
                        antList.push_back(newAnt);
                    }
                    break;
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    FoodSource newFoodSource = FoodSource(event.mouseButton.x, event.mouseButton.y);
                    foodList.push_back(newFoodSource);
                }
                break;
            }
            case sf::Event::KeyPressed:
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    drawTrails = !drawTrails;
                }
            }
        }
    }
}

void updateObjects(std::vector<Ant>& antList, PheromoneGrid &pheroGrid, std::vector<FoodSource>& foodList, std::vector<Home>& homeList, float deltaTime)
{
    // Each ant is processed
    for (int i = 0; i < antList.size(); i++)
    {
        antList[i].updateAnt(pheroGrid, foodList, homeList, deltaTime);
    }

    // Grid is processed
    pheroGrid.update(deltaTime);
}
void drawObjects(sf::RenderWindow& window, std::vector<Ant>& antList, PheromoneGrid &pheroGrid, std::vector<FoodSource>& foodList, std::vector<Home>& homeList)
{
    
    // First, clear the window
    window.clear(sf::Color(254, 217, 155));

    // Draw Phero Grid
    if (drawTrails)
    {
        window.draw(pheroGrid);
    }
    
    // Draw all ants
    for (int i = 0; i < antList.size(); i++)
    {
        window.draw(antList[i]);
    }

    // Draw Homes
    for (int i = 0; i < homeList.size(); i++)
    {
        window.draw(homeList[i]);
    }

    // Draw Food Sources
    for (int i = 0; i < foodList.size(); i++)
    {
        window.draw(foodList[i]);
    }

    

    // Finally, display drawn objects
    window.display();
}