#pragma once
#include <SFML/Graphics.hpp>

class Image;

class Tool {
public:
    virtual ~Tool() = default;

    virtual void onMousePressed(const sf::Vector2f& pos, Image* image, sf::Mouse::Button button = sf::Mouse::Left) = 0;
    virtual void onMouseReleased(const sf::Vector2f& pos, Image* image, sf::Mouse::Button button = sf::Mouse::Left) = 0;
    virtual void onMouseMoved(const sf::Vector2f& pos, Image* image) = 0;
};
