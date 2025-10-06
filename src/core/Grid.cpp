#include "Grid.hpp"
#include <cmath>

Grid::Grid() {}

void Grid::draw(sf::RenderWindow& window, const sf::Vector2f& imagePos, const sf::Vector2f& imageSize, float zoom) {
    if (!isVisible) return;
    
    generateGridLines(imagePos, imageSize, zoom);
    
    if (!gridLines.empty()) {
        window.draw(&gridLines[0], gridLines.size(), sf::Lines);
    }
}

void Grid::generateGridLines(const sf::Vector2f& imagePos, const sf::Vector2f& imageSize, float zoom) {
    gridLines.clear();
    
    float scaledGridSize = gridSize * zoom;
    
    if (scaledGridSize < 2.0f) return;
    
    float left = imagePos.x;
    float right = imagePos.x + imageSize.x;
    float top = imagePos.y;
    float bottom = imagePos.y + imageSize.y;
    
    float startX = std::ceil(left / scaledGridSize) * scaledGridSize;
    for (float x = startX; x <= right; x += scaledGridSize) {
        gridLines.push_back(sf::Vertex(sf::Vector2f(x, top), gridColor));
        gridLines.push_back(sf::Vertex(sf::Vector2f(x, bottom), gridColor));
    }
    
    float startY = std::ceil(top / scaledGridSize) * scaledGridSize;
    for (float y = startY; y <= bottom; y += scaledGridSize) {
        gridLines.push_back(sf::Vertex(sf::Vector2f(left, y), gridColor));
        gridLines.push_back(sf::Vertex(sf::Vector2f(right, y), gridColor));
    }
}
