#include "Game.h"
#include<iostream>


Game::Game() : m_window(sf::VideoMode(800, 590), "Snake Game"),
                m_world(sf::Vector2u(800, 590)),
                m_snake(16), 
                m_collectible(sf::Vector2u(800, 590)),
                m_obstacle1(16, sf::Vector2u(800, 590), 0.5f),  
                m_obstacle2(16, sf::Vector2u(800, 590), 0.5f)
{
    m_elapsed = 0.0f;
    m_window.setFramerateLimit(10);
}

Game::~Game()
{
	
}

void Game::HandleCollectible(Collectible& collectible)
{
    switch (collectible.GetType()) {
    case CollectibleType::HealthBoost:
        m_snake.IncreaseHealth();
        break;
    case CollectibleType::SpeedBoost:
        m_snake.IncreaseSpeed();
        break;
    case CollectibleType::ScoreBoost:
        m_snake.IncreaseScore();
        break;
    }
    collectible.RespawnShape(m_window.getSize());
}

void Game::HandleInput()
{
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            m_window.close();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_snake.GetDirection() != Direction::Down) {
            m_snake.SetDirection(Direction::Up);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_snake.GetDirection() != Direction::Up) {
            m_snake.SetDirection(Direction::Down);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && m_snake.GetDirection() != Direction::Right) {
            m_snake.SetDirection(Direction::Left);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && m_snake.GetDirection() != Direction::Left) {
            m_snake.SetDirection(Direction::Right);
        }
    }
}

void Game::Update()
{
    float timestep = 1.0f / m_snake.GetSpeed();
    m_elapsed += m_clock.restart().asSeconds();

    if (m_elapsed >= timestep) {
        m_snake.Tick();
        m_world.Update(m_snake);
        m_elapsed -= timestep;
        m_collectible.Update(m_snake);
        m_obstacle1.Update(m_elapsed);
        m_obstacle2.Update(m_elapsed);

        if (m_snake.GetPosition() == m_obstacle1.GetPosition() 
            || m_snake.GetPosition() == m_obstacle2.GetPosition()) 
        {
            m_snake.Lose();
        }

        if (m_snake.HasLost()) {
            m_snake.Reset();     
        }
    }
}

void Game::Render()
{
    this->m_window.clear();

    this->m_world.Render(m_window);
    this->m_snake.Render(m_window); 
    this->m_collectible.Render(m_window);
    m_obstacle1.Render(m_window);
    m_obstacle2.Render(m_window);


    m_window.display();
}

void Game::Run()
{
    while (m_window.isOpen()) {
        HandleInput();
        Update();
        Render();
    }
}
