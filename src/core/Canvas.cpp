#include "Canvas.hpp"

Canvas::Canvas(int width, int height) {
    background.setSize(sf::Vector2f(width, height));
    background.setFillColor(sf::Color::White);
    background.setPosition(0, 0);
}

void Canvas::draw(sf::RenderWindow& window) {
    window.draw(background);
}
