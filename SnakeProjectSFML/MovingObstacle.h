
#pragma once

#include <SFML/Graphics.hpp>

class MovingObstacle
{
private:
    sf::RectangleShape m_shape;
    sf::Vector2i m_position;
    sf::Vector2i m_direction;
    float m_speed;
    float m_blockSize;
    sf::Vector2u m_windowSize;

public:
    MovingObstacle(float l_blockSize, sf::Vector2u l_windowSize, float speed);
    void Move();
    void Render(sf::RenderWindow& window);
    void Update(float deltaTime);
    sf::Vector2i GetPosition() const;
    void Respawn();
};


