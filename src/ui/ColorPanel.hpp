#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "../core/ColorManager.hpp"

class ColorPanel {
public:
    ColorPanel();
    void draw(sf::RenderWindow& window);
    bool handleClick(const sf::Vector2f& mousePos, sf::Mouse::Button button);
    sf::FloatRect getBounds() const { return bounds; }
    void setPosition(const sf::Vector2f& pos);
    void setOnOpenPicker(std::function<void(bool isForeground)> cb) { onOpenPicker = cb; }

private:
    sf::RectangleShape fgRect;
    sf::RectangleShape bgRect;
    sf::FloatRect bounds;
    std::function<void(bool)> onOpenPicker;
};
