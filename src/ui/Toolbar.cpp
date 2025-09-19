#include "Toolbar.hpp"

void Toolbar::addButton(const Button& button) {
    buttons.push_back(button);
}

void Toolbar::draw(sf::RenderWindow& window) {
    for (auto& btn : buttons) {
        btn.draw(window);
    }
}

void Toolbar::handleClick(const sf::Vector2f& mousePos) {
    for (auto& btn : buttons) {
        btn.onClick(mousePos);
    }
}
