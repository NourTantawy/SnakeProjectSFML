
#include "MovingObstacle.h"
#include <cstdlib>  // for rand()


MovingObstacle::MovingObstacle(float l_blockSize, sf::Vector2u l_windowSize, float speed)
    : m_blockSize(l_blockSize), m_windowSize(l_windowSize), m_speed(speed)
{
    m_shape.setSize(sf::Vector2f(m_blockSize, m_blockSize));
    m_shape.setFillColor(sf::Color::Magenta);  // Color for obstacles
    Respawn();
}

void MovingObstacle::Respawn()
{
    // Random position within bounds, ensuring obstacles are within the grid size
    int maxX = (m_windowSize.x / m_blockSize) - 2;
    int maxY = (m_windowSize.y / m_blockSize) - 2;

    // Randomize obstacle position
    m_position.x = rand() % maxX + 1;
    m_position.y = rand() % maxY + 1;

    // Randomize direction (either -1 or +1)
    m_direction.x = (rand() % 2 == 0) ? 1 : -1;  // Random horizontal direction
    m_direction.y = (rand() % 2 == 0) ? 1 : -1;  // Random vertical direction

    // Set the shape's position
    m_shape.setPosition(m_position.x * m_blockSize, m_position.y * m_blockSize);
}

void MovingObstacle::Move()
{
    // Move the obstacle based on its direction and speed
    m_position.x += m_direction.x * m_speed;
    m_position.y += m_direction.y * m_speed;

    // Boundary collision detection (we ensure it stays within the window)
    if (m_position.x <= 1) {  // Don't let it go past the left side
        m_position.x = 1;
        m_direction.x = -m_direction.x;  // Reverse horizontal direction
    }

    if (m_position.x >= m_windowSize.x / m_blockSize - 2) {  // Don't let it go past the right side
        m_position.x = m_windowSize.x / m_blockSize - 2;
        m_direction.x = -m_direction.x;  // Reverse horizontal direction
    }

    if (m_position.y <= 1) {  // Don't let it go past the top side
        m_position.y = 1;
        m_direction.y = -m_direction.y;  // Reverse vertical direction
    }

    if (m_position.y >= m_windowSize.y / m_blockSize - 2) {  // Don't let it go past the bottom side
        m_position.y = m_windowSize.y / m_blockSize - 2;
        m_direction.y = -m_direction.y;  // Reverse vertical direction
    }

    // Update the obstacle's shape position for rendering
    m_shape.setPosition(m_position.x * m_blockSize, m_position.y * m_blockSize);
}


void MovingObstacle::Render(sf::RenderWindow& window)
{
    window.draw(m_shape);  // Draw the obstacle at its current position
}

void MovingObstacle::Update(float deltaTime)
{
    // Move the obstacle based on the deltaTime for smooth movement
    // We use deltaTime to scale the speed, making the movement frame-rate independent
    Move();
}

sf::Vector2i MovingObstacle::GetPosition() const
{
    return m_position;
}
