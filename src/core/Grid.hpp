#pragma once
#include <SFML/Graphics.hpp>

class Grid {
public:
    Grid();
    
    void draw(sf::RenderWindow& window, const sf::Vector2f& imagePos, const sf::Vector2f& imageSize, float zoom);
    void setVisible(bool visible) { isVisible = visible; }
    bool getVisible() const { return isVisible; }
    void setGridSize(float size) { gridSize = size; }
    float getGridSize() const { return gridSize; }
    void setColor(const sf::Color& color) { gridColor = color; }

private:
    bool isVisible = false;
    float gridSize = 20.0f; // pixels
    sf::Color gridColor = sf::Color(128, 128, 128, 100);
    std::vector<sf::Vertex> gridLines;
    
    void generateGridLines(const sf::Vector2f& imagePos, const sf::Vector2f& imageSize, float zoom);
};
