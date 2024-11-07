#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "World.h"

enum class CollectibleType { HealthBoost, SpeedBoost, ScoreBoost };

class Collectible
{
private:
	CollectibleType m_type;
	sf::CircleShape m_shape;
	sf::Vector2i m_item;
	int m_blockSize = 16;

public:
	Collectible(sf::Vector2u l_windowSize);
	void RespawnShape(sf::Vector2u l_windowSize);
	void Render(sf::RenderWindow& l_window);
	void Update(Snake& l_player);
	CollectibleType GetType();
};

