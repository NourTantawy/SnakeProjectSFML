#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>

using namespace sf;

struct Map {
    std::vector<std::string> mapData; // Stores the map from the file
    std::vector<RectangleShape> boundaries; // Stores boundary shapes
    const int cellSize = 25;

    Map(const std::string& filename) {
        loadMapFromFile(filename);
    }

    void loadMapFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open map file: " << filename << std::endl;
            return;
        }

        std::string line;
        int y = 0;

        while (std::getline(file, line)) {
            mapData.push_back(line);
            for (int x = 0; x < line.size(); ++x) {
                if (line[x] == '1') { // '1' represents a boundary
                    RectangleShape boundary(Vector2f(cellSize, cellSize));
                    boundary.setFillColor(Color::Red); // Color for boundary
                    boundary.setPosition(x * cellSize, y * cellSize);
                    boundaries.push_back(boundary);
                }
            }
            y++;
        }
    }
};  

// Constants
const int BLOCK_SIZE = 16;
const sf::Vector2u WINDOW_SIZE(800, 590);

// Global Variables
sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "Snake Game");

std::vector<sf::Vector2i> snakeBody;
sf::Vector2i snakeDirection(1, 0); // Initial snake direction
int snakeSpeed = 10;
int snakeHealth = 3;
int snakeScore = 0;

sf::CircleShape collectibleShape;
sf::Vector2i collectiblePosition;
int collectibleType;

sf::RectangleShape obstacle1Shape;
sf::Vector2i obstacle1Position;
sf::Vector2i obstacle1Direction(1, 0);
float obstacle1Speed = 0.5f;

sf::RectangleShape obstacle2Shape;
sf::Vector2i obstacle2Position;
sf::Vector2i obstacle2Direction(1, 0);
float obstacle2Speed = 0.5f;

std::vector<sf::RectangleShape> boundaries;

// Function Prototypes
void InitializeGame();
void HandleInput();
void MoveSnake();
void ExtendSnake();
void MoveObstacle(sf::Vector2i& position, sf::Vector2i& direction, float speed, sf::RectangleShape& shape);
void RespawnCollectible();
void CheckCollisions();
void Render();
void Update();

// Game Loop
int main() {
    InitializeGame();

    sf::Clock clock;
    float elapsed = 0.0f;

    while (window.isOpen()) {
        HandleInput();

        elapsed += clock.restart().asSeconds();
        if (elapsed >= 1.0f / snakeSpeed) {
            Update();
            elapsed = 0.0f;
        }

        Render();
    }

    return 0;
}

// Function Definitions

void InitializeGame() {
    // Initialize snake
    snakeBody.push_back(sf::Vector2i(10, 10));

    // Initialize collectible
    RespawnCollectible();

    // Initialize obstacles
    obstacle1Shape.setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    obstacle1Shape.setFillColor(sf::Color::Magenta);
    obstacle1Position = sf::Vector2i(rand() % (WINDOW_SIZE.x / BLOCK_SIZE - 2) + 1, rand() % (WINDOW_SIZE.y / BLOCK_SIZE - 2) + 1);
    obstacle1Shape.setPosition(obstacle1Position.x * BLOCK_SIZE, obstacle1Position.y * BLOCK_SIZE);

    obstacle2Shape.setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    obstacle2Shape.setFillColor(sf::Color::Magenta);
    obstacle2Position = sf::Vector2i(rand() % (WINDOW_SIZE.x / BLOCK_SIZE - 2) + 1, rand() % (WINDOW_SIZE.y / BLOCK_SIZE - 2) + 1);
    obstacle2Shape.setPosition(obstacle2Position.x * BLOCK_SIZE, obstacle2Position.y * BLOCK_SIZE);

    // Initialize window settings
    window.setFramerateLimit(10);

    // Load boundaries from file
    std::ifstream file("boundary.txt");
    std::string line;
    int row = 0;
    boundaries.clear();
    while (std::getline(file, line)) {
        for (int col = 0; col < line.size(); ++col) {
            if (line[col] == '1') {  // '1' represents a boundary block
                sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
                block.setPosition(col * BLOCK_SIZE, row * BLOCK_SIZE);
                block.setFillColor(sf::Color(150, 0, 0));
                boundaries.push_back(block);
            }
        }
        ++row;
    }
}

void HandleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && snakeDirection != sf::Vector2i(0, 1)) {
        snakeDirection = sf::Vector2i(0, -1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && snakeDirection != sf::Vector2i(0, -1)) {
        snakeDirection = sf::Vector2i(0, 1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && snakeDirection != sf::Vector2i(1, 0)) {
        snakeDirection = sf::Vector2i(-1, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && snakeDirection != sf::Vector2i(-1, 0)) {
        snakeDirection = sf::Vector2i(1, 0);
    }
}

void MoveSnake() {
    for (size_t i = snakeBody.size() - 1; i > 0; --i) {
        snakeBody[i] = snakeBody[i - 1];
    }
    snakeBody[0] += snakeDirection;
}

void ExtendSnake() {
    snakeBody.push_back(snakeBody.back());
}

void MoveObstacle(sf::Vector2i& position, sf::Vector2i& direction, float speed, sf::RectangleShape& shape) {
    position.x += direction.x * speed;
    position.y += direction.y * speed;

    // Boundary collision detection for obstacles
    if (position.x <= 1 || position.x >= WINDOW_SIZE.x / BLOCK_SIZE - 2) {
        direction.x = -direction.x;
    }
    if (position.y <= 1 || position.y >= WINDOW_SIZE.y / BLOCK_SIZE - 2) {
        direction.y = -direction.y;
    }

    shape.setPosition(position.x * BLOCK_SIZE, position.y * BLOCK_SIZE);
}

void RespawnCollectible() {
    collectiblePosition.x = rand() % (WINDOW_SIZE.x / BLOCK_SIZE - 2) + 1;
    collectiblePosition.y = rand() % (WINDOW_SIZE.y / BLOCK_SIZE - 2) + 1;
    collectibleType = rand() % 3;
    collectibleShape.setPosition(collectiblePosition.x * BLOCK_SIZE, collectiblePosition.y * BLOCK_SIZE);
    collectibleShape.setRadius(BLOCK_SIZE / 2);

    if (collectibleType == 0) {
        collectibleShape.setFillColor(sf::Color::Red); // Health Boost
    }
    else if (collectibleType == 1) {
        collectibleShape.setFillColor(sf::Color::White); // Speed Boost
    }
    else {
        collectibleShape.setFillColor(sf::Color::Cyan); // Score Boost
    }
}

void CheckCollisions() {
    // Boundary collisions
    if (snakeBody[0].x <= 0 || snakeBody[0].y <= 0 ||
        snakeBody[0].x >= WINDOW_SIZE.x / BLOCK_SIZE - 1 || snakeBody[0].y >= WINDOW_SIZE.y / BLOCK_SIZE - 1) {
        snakeHealth = 0;  // Player loses
    }

    // Collectible collision
    if (snakeBody[0] == collectiblePosition) {
        ExtendSnake();
        RespawnCollectible();
    }

    // Obstacle collision
    if (snakeBody[0] == obstacle1Position || snakeBody[0] == obstacle2Position) {
        snakeHealth = 0;  // Player loses
    }
}

void Render() {
    window.clear();

    // Render boundaries
    for (auto& block : boundaries) {
        window.draw(block);
    }

    // Render snake
    for (auto& segment : snakeBody) {
        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
        block.setPosition(segment.x * BLOCK_SIZE, segment.y * BLOCK_SIZE);
        block.setFillColor(sf::Color::Green);
        window.draw(block);
    }

    // Render collectible
    window.draw(collectibleShape);

    // Render obstacles
    window.draw(obstacle1Shape);
    window.draw(obstacle2Shape);

    window.display();
}

void Update() {
    MoveSnake();
    MoveObstacle(obstacle1Position, obstacle1Direction, obstacle1Speed, obstacle1Shape);
    MoveObstacle(obstacle2Position, obstacle2Direction, obstacle2Speed, obstacle2Shape);

    CheckCollisions();
}
