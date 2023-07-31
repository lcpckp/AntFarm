#include<iostream>
#include<thread>
#include<SFML/Graphics.hpp>
#include"Home.h"
#include"Ant.h"
#include"Resources.h"
#include"PheromoneGrid.h"
#include"FoodSource.h"


bool drawTrails = true;
bool runMultiThreaded = true;

void mainLoop(sf::Clock& clock, sf::RenderWindow& window, std::vector<Ant>& antList, std::vector<FoodSource>& foodList, std::vector<Home>& homeList, PheromoneGrid& pheroGrid, float deltaTime);

void handleInput(sf::RenderWindow &window, std::vector<Ant> &antList, std::vector<FoodSource>& foodList, PheromoneGrid& pheroGrid);
void updateObjects(sf::Clock& clock, sf::RenderWindow& window, std::vector<Ant>& antList, std::vector<FoodSource>& foodList, std::vector<Home>& homeList, PheromoneGrid& pheroGrid, float deltaTime);
void updateObjectsThread(sf::Clock& clock, sf::RenderWindow& window, std::vector<Ant>& antList, std::vector<FoodSource>& foodList, std::vector<Home>& homeList, PheromoneGrid& pheroGrid, float deltaTime);
void drawObjects(sf::RenderWindow &window, std::vector<Ant> &antList, std::vector<FoodSource>& foodList, std::vector<Home>& homeList, PheromoneGrid& pheroGrid);
void spawnAnts(sf::Event& event, std::vector<Ant>& antList, PheromoneGrid& pheroGrid);
void spawnFood(sf::Event& event, std::vector<FoodSource>& foodList);

int main()
{
    Resources resourceContainer = Resources(); // Instantiate resources container which loads fonts, colors, textures, etc.
    
    // Render Window setup
    sf::RenderWindow window(sf::VideoMode(Resources::farmWidth, Resources::farmHeight), "Ant Farm", sf::Style::Close);
    sf::Image icon;
    icon.loadFromFile("res\\ant_2.png");
    window.setIcon(32, 32, icon.getPixelsPtr());
    //window.setFramerateLimit(60);

    // Simulation objects
    std::vector<Ant> antList;
    std::vector<FoodSource> foodList;
    std::vector<Home> homeList;
    PheromoneGrid pheroGrid;

    sf::Clock clock;
    float deltaTime = 0.0f;

    // Temporary start conditions
    homeList.push_back(Home(Resources::farmWidth / 2, Resources::farmHeight / 2));
    for (int i = 0; i < Resources::numStartingAnts; i++)
    {
        Ant newAnt = Ant(Resources::farmWidth / 2, Resources::farmHeight / 2, pheroGrid);
        antList.push_back(newAnt);
    }
    
    // Run simulation on initialized objects
    mainLoop(clock, window, antList, foodList, homeList, pheroGrid, deltaTime);

	return 0;
}

void mainLoop(sf::Clock& clock, sf::RenderWindow& window, std::vector<Ant>& antList, std::vector<FoodSource>& foodList, std::vector<Home>& homeList, PheromoneGrid& pheroGrid, float deltaTime)
{
    if (runMultiThreaded)
    {
        // Update objects on new thread
        std::thread worker(updateObjectsThread, std::ref(clock), std::ref(window), std::ref(antList), std::ref(foodList), std::ref(homeList), std::ref(pheroGrid), std::ref(deltaTime));
        
        while (window.isOpen())
        {
            // Handle Inputs
            handleInput(window, antList, foodList, pheroGrid);

            // Draw objects
            drawObjects(window, antList, foodList, homeList, pheroGrid);
        }

        worker.join();
    }
    else
    {
        while (window.isOpen())
        {
            deltaTime = clock.restart().asSeconds();

            // Handle Inputs
            handleInput(window, antList, foodList, pheroGrid);

            // Update Objects
            updateObjects(clock, window, antList, foodList, homeList, pheroGrid, deltaTime);

            // Draw objects
            drawObjects(window, antList, foodList, homeList, pheroGrid);
        }
    }
    
    
    

    
}

void handleInput(sf::RenderWindow &window, std::vector<Ant> &antList, std::vector<FoodSource>& foodList, PheromoneGrid& pheroGrid)
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
                    spawnAnts(event, antList, pheroGrid);
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    spawnFood(event, foodList);
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

void spawnFood(sf::Event& event, std::vector<FoodSource>& foodList)
{
    if (foodList.size() < Resources::maxFoodSources)
    {
        FoodSource newFoodSource = FoodSource(event.mouseButton.x, event.mouseButton.y);
        foodList.push_back(newFoodSource);
    }
    else
    {
        std::cout << "Too many food sources" << std::endl;
    }
}

void spawnAnts(sf::Event& event, std::vector<Ant>& antList, PheromoneGrid& pheroGrid)
{
    for (int i = 0; i < Resources::numAntsPerClick; i++)
    {
        Ant newAnt = Ant(event.mouseButton.x, event.mouseButton.y, pheroGrid);
        antList.push_back(newAnt);
    }
}

void updateObjectsThread(sf::Clock& clock, sf::RenderWindow& window, std::vector<Ant>& antList, std::vector<FoodSource>& foodList, std::vector<Home>& homeList, PheromoneGrid& pheroGrid, float deltaTime)
{
    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();

        // Each ant is processed
        for (std::vector<Ant>::iterator it = antList.begin(); it != antList.end(); ++it)
        {
            it->updateAnt(pheroGrid, foodList, homeList, deltaTime);
        }

        // Each food source is processed
        for (std::vector<FoodSource>::iterator it = foodList.begin(); it != foodList.end(); ++it)
        {
            if (!it->hasFood())
            {
                foodList.erase(it);
                break;
            }
        }

        // Grid is processed
        pheroGrid.update(deltaTime);
    }
}

void updateObjects(sf::Clock& clock, sf::RenderWindow& window, std::vector<Ant>& antList, std::vector<FoodSource>& foodList, std::vector<Home>& homeList, PheromoneGrid& pheroGrid, float deltaTime)
{
    // Each ant is processed
    for (std::vector<Ant>::iterator it = antList.begin(); it != antList.end(); ++it)
    {
        it->updateAnt(pheroGrid, foodList, homeList, deltaTime);
    }

    // Each food source is processed
    for (std::vector<FoodSource>::iterator it = foodList.begin(); it != foodList.end(); ++it)
    {
        if (!it->hasFood())
        {
            foodList.erase(it);
            break;
        }
    }

    // Grid is processed
    pheroGrid.update(deltaTime);
}

void drawObjects(sf::RenderWindow& window, std::vector<Ant>& antList, std::vector<FoodSource>& foodList, std::vector<Home>& homeList, PheromoneGrid& pheroGrid)
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