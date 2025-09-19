#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class Button {
public:
    Button(const sf::Vector2f& pos, const sf::Vector2f& size, const std::string& text);

    void draw(sf::RenderWindow& window);
    bool isHovered(const sf::Vector2f& mousePos) const;
    bool isClicked(const sf::Vector2f& mousePos) const;

    void setCallback(std::function<void()> cb);

private:
    sf::RectangleShape shape;
    sf::Text label;
    std::function<void()> callback;
    static sf::Font font;
    static bool fontLoaded;
public:
    void onClick(const sf::Vector2f& mousePos);
};
