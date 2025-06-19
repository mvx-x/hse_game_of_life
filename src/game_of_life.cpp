#include <SFML/Graphics.hpp>
#include <random>

#include "game_of_life.h"

GameOfLife::GameOfLife(int width, int height)
    : width(width),
    height(height),
    grid(height, std::vector<bool>(width, false)),
    nextGrid(height, std::vector<bool>(width, false)),
    immortal(height, std::vector<bool>(width, false)) {
}

void GameOfLife::randomize() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x] = (dis(gen) == 1);
            immortal[y][x] = false;
        }
    }
}

void GameOfLife::clear() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x] = false;
            immortal[y][x] = false;
        }
    }
}

void GameOfLife::toggleCell(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x] = !grid[y][x];
    }
}

void GameOfLife::toggleImmortal(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        if (grid[y][x]) {
            immortal[y][x] = !immortal[y][x];
        }
    }
}

bool GameOfLife::isImmortal(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return immortal[y][x];
    }
    return false;
}

void GameOfLife::update() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (immortal[y][x]) {
                nextGrid[y][x] = true;
            }
            else {
                int liveNeighbors = countLiveNeighbors(x, y);
                if (grid[y][x]) {
                    nextGrid[y][x] = (liveNeighbors == 2 || liveNeighbors == 3);
                }
                else {
                    nextGrid[y][x] = (liveNeighbors == 3);
                }
            }
        }
    }
    grid.swap(nextGrid);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            nextGrid[y][x] = false;
        }
    }
    ++generationCount;
}

bool GameOfLife::isCellAlive(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return grid[y][x];
    }
    return false;
}

std::pair<int, int> GameOfLife::getGridSize() const { return { width, height }; }

int GameOfLife::countLiveNeighbors(int x, int y) const {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                if (grid[ny][nx]) {
                    ++count;
                }
            }
        }
    }
    return count;
}

void GameOfLife::setCellColor(const sf::Color& color) { cellColor = color; }

void GameOfLife::draw(sf::RenderWindow& window, int cellSize) const {
    sf::RectangleShape cellShape(sf::Vector2f(static_cast<float>(cellSize - 1),
        static_cast<float>(cellSize - 1)));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x]) {
                if (immortal[y][x]) {
                    cellShape.setFillColor(sf::Color::White);
                }
                else {
                    cellShape.setFillColor(cellColor);
                }
                cellShape.setPosition(sf::Vector2f(static_cast<float>(x * cellSize),
                    static_cast<float>(y * cellSize)));
                window.draw(cellShape);
            }
        }
    }
}

int GameOfLife::getGenerationCount() const { return generationCount; }

void GameOfLife::resetGenerationCount() { generationCount = 0; }

int GameOfLife::getLiveCellCount() const {
    int count = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x]) {
                ++count;
            }
        }
    }
    return count;
}

int GameOfLife::getDeadCellCount() const {
    return (width * height) - getLiveCellCount();
}

double GameOfLife::getLiveCellPercentage() const {
    int totalCells = width * height;
    if (totalCells == 0) return 0.0;

    int liveCells = getLiveCellCount();
    return (static_cast<double>(liveCells) / totalCells) * 100.0;
}