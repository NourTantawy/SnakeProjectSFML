#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "World.h"
#include "Snake.h"
#include "Collectible.h"
#include "MovingObstacle.h"


class Game {
private:
	sf::RenderWindow m_window;
	World m_world;
	Snake m_snake;
	Collectible m_collectible;
	MovingObstacle m_obstacle1;  
	MovingObstacle m_obstacle2; 

	sf::Clock m_clock;
	float m_elapsed;

public:
	Game();
	~Game();

	void HandleCollectible(Collectible& collectible);
	void HandleInput();
	void Update();
	void Render();
	void Run();

};

