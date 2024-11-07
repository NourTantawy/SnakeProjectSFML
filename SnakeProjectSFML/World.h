#pragma once

#include <fstream>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Snake.h"

class World
{
private:
	sf::Vector2u m_windowSize;
	sf::Vector2i m_item;
	int m_blockSize;

	std::vector<sf::RectangleShape> m_boundaries;

public:
	World(sf::Vector2u l_windSize);
	~World();

	int GetBlockSize();

	void LoadBoundary(const std::string& filename);

	void Update(Snake& l_player);
	void Render(sf::RenderWindow& l_window);
};

