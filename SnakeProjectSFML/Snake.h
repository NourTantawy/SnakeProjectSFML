#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>




struct SnakeSegment {
	sf::Vector2i position;
	SnakeSegment(int x, int y) : position(x, y) {}
};

using SnakeContainer = std::vector<SnakeSegment>;

enum class Direction {None, Up, Down, Left, Right};



class Snake
{
private:
	SnakeContainer m_snakeBody;
	int m_size;
	Direction m_dir;
	int m_speed;
	int m_health;
	int m_score;
	bool m_lost;
	sf::RectangleShape m_bodyRect;

public:
	// Constructor-Destructor
	Snake(int l_blockSize);
	~Snake();
	
	// Helper methods
	void SetDirection(Direction l_dir);
	Direction GetDirection();
	int GetSpeed();
	sf::Vector2i GetPosition();
	int GetHealth();
	int GetScore();
	void IncreaseScore();
	void IncreaseHealth();
	void IncreaseSpeed();
	bool HasLost();

	void Lose();
	void ToggleLost();

	void Extend();
	void Reset();

	void Move();
	void Tick();
	void Cut(int l_segments);
	void Render(sf::RenderWindow& l_window);

	void CheckCollision();
};

