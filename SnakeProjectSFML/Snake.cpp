#include "Snake.h"


Snake::Snake(int l_blockSize)
{
	this->m_size = l_blockSize;
	this->m_bodyRect.setSize(sf::Vector2f(m_size - 1, m_size - 1));
	Reset();
}

Snake::~Snake()
{

}

void Snake::SetDirection(Direction l_dir)
{
	this->m_dir = l_dir;
}

Direction Snake::GetDirection()
{
	return this->m_dir;
}

int Snake::GetSpeed()
{
	return this->m_speed;
}

sf::Vector2i Snake::GetPosition()
{
	if (!this->m_snakeBody.empty())
		return this->m_snakeBody.front().position;
	else
		return sf::Vector2i(1, 1);
}

int Snake::GetHealth()
{
	return this->m_health;
}

int Snake::GetScore()
{
	return this->m_score;
}

void Snake::IncreaseScore()
{
	this->m_score += 10;
}

void Snake::IncreaseHealth()
{
	this->m_health += 1;
}

void Snake::IncreaseSpeed()
{
	this->m_speed += 5;
}

bool Snake::HasLost()
{
	return this->m_lost;
}

void Snake::Lose()
{
	this->m_lost = true;
}

void Snake::ToggleLost()
{
	this->m_lost = !this->m_lost;
}

void Snake::Extend()
{
	if (this->m_snakeBody.empty()) return;

	SnakeSegment& tail_head = this->m_snakeBody[this->m_snakeBody.size() - 1];

	if (this->m_snakeBody.size() > 1) 
	{
		SnakeSegment& tail_bone = this->m_snakeBody[this->m_snakeBody.size() - 2];
		
		if (tail_head.position.x == tail_bone.position.x) {
			if (tail_head.position.y > tail_bone.position.y) {
				this->m_snakeBody.push_back(SnakeSegment(tail_head.position.x, tail_head.position.y + 1));
			} else {
				this->m_snakeBody.push_back(SnakeSegment(tail_head.position.x, tail_head.position.y - 1));
			}
		} else if (tail_head.position.y == tail_bone.position.y) {
			if (tail_head.position.x > tail_bone.position.x) {
				this->m_snakeBody.push_back(SnakeSegment(tail_head.position.x + 1, tail_head.position.y));
			}
			else {
				this->m_snakeBody.push_back(SnakeSegment(tail_head.position.x - 1, tail_head.position.y));
			}
		}
	} else {
		if (this->m_dir == Direction::Up) {
			this->m_snakeBody.push_back(SnakeSegment(tail_head.position.x, tail_head.position.y + 1));
		} else if (this->m_dir == Direction::Down) {
			this->m_snakeBody.push_back(SnakeSegment(tail_head.position.x, tail_head.position.y - 1));
		} else if (this->m_dir == Direction::Left) {
			this->m_snakeBody.push_back(SnakeSegment(tail_head.position.x + 1, tail_head.position.y));
		} else if (this->m_dir == Direction::Right) {
			this->m_snakeBody.push_back(SnakeSegment(tail_head.position.x - 1, tail_head.position.y));
		}
	}
}

void Snake::Reset()
{
	this->m_snakeBody.clear(); // empty the vector

	this->m_snakeBody.push_back(SnakeSegment(5, 7));
	this->m_snakeBody.push_back(SnakeSegment(5, 6));
	this->m_snakeBody.push_back(SnakeSegment(5, 5));

	this->SetDirection(Direction::None);
	this->m_speed = 10;
	this->m_health = 3;
	this->m_score = 0;
	this->m_lost = false;
}

void Snake::Move()
{
	for (int i = this->m_snakeBody.size() - 1; i > 0; --i) {
		this->m_snakeBody[i].position = this->m_snakeBody[i - 1].position;
	}

	if (this->m_dir == Direction::Up) {
		--(this->m_snakeBody[0].position.y);
	} else if (this->m_dir == Direction::Down) {
		++(this->m_snakeBody[0].position.y);
	} else if (this->m_dir == Direction::Left) {
		--(this->m_snakeBody[0].position.x);
	} else if (this->m_dir == Direction::Right) {
		++(this->m_snakeBody[0].position.x);
	}
}

void Snake::Tick()
{
	if (this->m_snakeBody.empty()) return;
	if (this->m_dir == Direction::None) return;
	Move();
	CheckCollision();
}

void Snake::Cut(int l_segments)
{
	for (int i = 0; i < l_segments; ++i) {
		this->m_snakeBody.pop_back();
	}
	--(this->m_health);
	if (!this->m_health) {
		Lose();
		return;
	}
}

void Snake::Render(sf::RenderWindow& l_window)
{
	if (this->m_snakeBody.empty()) return;

	m_bodyRect.setFillColor(sf::Color::Yellow);
	m_bodyRect.setPosition(m_snakeBody[0].position.x * m_size, m_snakeBody[0].position.y * m_size);
	l_window.draw(m_bodyRect);

	m_bodyRect.setFillColor(sf::Color::Green);
	for (size_t i = 1; i < m_snakeBody.size(); ++i) {
		m_bodyRect.setPosition(m_snakeBody[i].position.x * m_size, m_snakeBody[i].position.y * m_size);
		l_window.draw(m_bodyRect);
	}
}

void Snake::CheckCollision()
{
	if (this->m_snakeBody.size() < 5) return;

	SnakeSegment& head = this->m_snakeBody.front();

	for (auto itr = this->m_snakeBody.begin() + 1; itr != this->m_snakeBody.end(); ++itr) 
	{
		if (itr->position == head.position) {
			int segments = this->m_snakeBody.end() - itr;
			Cut(segments);
			break;
		}
	}
}
