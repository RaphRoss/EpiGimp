#pragma once
#include "Button.hpp"
#include <vector>

class Toolbar {
public:
    void addButton(const Button& button);
    void draw(sf::RenderWindow& window);
    void handleClick(const sf::Vector2f& mousePos);

private:
    std::vector<Button> buttons;
};
