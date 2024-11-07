#include "World.h"

World::World(sf::Vector2u l_windSize)
{
	this->m_blockSize = 16;

	this->m_windowSize = l_windSize;

	this->LoadBoundary("boundary.txt"); 
}

World::~World()
{
}

int World::GetBlockSize()
{
	return this->m_blockSize;
}

void World::LoadBoundary(const std::string& filename)
{
	std::ifstream file(filename);
	std::string line;
	int row = 0;

	m_boundaries.clear();
	while (std::getline(file, line)) {
		for (int col = 0; col < line.size(); ++col) {
			if (line[col] == '1') {  // '1' represents a boundary block
				sf::RectangleShape block(sf::Vector2f(m_blockSize, m_blockSize));
				block.setPosition(col * m_blockSize, row * m_blockSize);
				block.setFillColor(sf::Color(150, 0, 0));
				m_boundaries.push_back(block);
			}
		}
		++row;
	}
}

void World::Update(Snake& l_player)
{
	int gridSize_x = this->m_windowSize.x / this->m_blockSize;
	int gridSize_y = this->m_windowSize.y / this->m_blockSize;

	if (l_player.GetPosition().x <= 0 || l_player.GetPosition().y <= 0
		|| l_player.GetPosition().x >= gridSize_x - 1 || l_player.GetPosition().y >= gridSize_y - 1) 
	{
		l_player.Lose();
	}
}

void World::Render(sf::RenderWindow& l_window)
{
	for (int i = 0; i < this->m_boundaries.size(); ++i) 
	{
		l_window.draw(this->m_boundaries[i]);
	}
}
