#include "Collectible.h"

#include<iostream>
using namespace std;



Collectible::Collectible(sf::Vector2u l_windowSize) 
{
    this->RespawnShape(l_windowSize);    
}

void Collectible::RespawnShape(sf::Vector2u l_windowSize) 
{
    int maxX = (l_windowSize.x / m_blockSize) - 2;
    int maxY = (l_windowSize.y / m_blockSize) - 2;

    this->m_item = sf::Vector2i(rand() % maxX + 1, rand() % maxY + 1);
    m_shape.setPosition(this->m_item.x * m_blockSize, this->m_item.y * m_blockSize);

    int type = rand() % 3;

    if (type == 0) {
        m_type = CollectibleType::HealthBoost;
        m_shape.setFillColor(sf::Color::Red);
    }
    else if (type == 1) {
        m_type = CollectibleType::SpeedBoost;
        m_shape.setFillColor(sf::Color::White);
    }
    else if (type == 2) {
        m_type = CollectibleType::ScoreBoost;
        m_shape.setFillColor(sf::Color::Cyan);
    }

    this->m_shape.setRadius(m_blockSize / 2);
}

void Collectible::Render(sf::RenderWindow& l_window)
{
	l_window.draw(this->m_shape);
}

void Collectible::Update(Snake& l_player)
{
    if (l_player.GetPosition() == this->m_item)
    {
        l_player.Extend();

        switch (m_type) {
        case CollectibleType::HealthBoost:
            l_player.IncreaseHealth();
            break;
        case CollectibleType::SpeedBoost:
            l_player.IncreaseSpeed();
            break;
        case CollectibleType::ScoreBoost:
            l_player.IncreaseScore();
            break;
        }

        cout << "health: " << l_player.GetHealth() << endl;
        cout << "score: " << l_player.GetScore() << endl;
        cout << "speed: " << l_player.GetSpeed() << endl << endl;

        this->RespawnShape(sf::Vector2u(800, 590));
    }
}

CollectibleType Collectible::GetType()
{
    return this->m_type; 
}
