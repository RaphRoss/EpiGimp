#pragma once
#include <SFML/Graphics.hpp>

class Tool {
public:
    virtual ~Tool() = default;

    virtual void onMousePressed(const sf::Vector2f& pos) = 0;
    virtual void onMouseReleased(const sf::Vector2f& pos) = 0;
    virtual void onMouseMoved(const sf::Vector2f& pos) = 0;
};
