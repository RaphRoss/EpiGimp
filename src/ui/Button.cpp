#include "Button.hpp"
#include <iostream>

sf::Font Button::font;
bool Button::fontLoaded = false;

Button::Button(const sf::Vector2f& pos, const sf::Vector2f& size, const std::string& text) {
    shape.setPosition(pos);
    shape.setSize(size);
    shape.setFillColor(sf::Color(200, 200, 200));
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2);
    if (!fontLoaded) {
        if (!font.loadFromFile("src/assets/fonts/DejaVuSans-Bold.ttf")) {
            std::cerr << "Erreur: Impossible de charger la police !" << std::endl;
            fontLoaded = false;
        } else {
            fontLoaded = true;
        }
    }
    if (fontLoaded) {
        label.setFont(font);
    }
    label.setString(text);
    label.setCharacterSize(14);
    label.setFillColor(sf::Color::Black);

    sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
    label.setPosition(pos.x + size.x / 2, pos.y + size.y / 2);
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    if (fontLoaded) {
        window.draw(label);
    }
}

bool Button::isHovered(const sf::Vector2f& mousePos) const {
    return shape.getGlobalBounds().contains(mousePos);
}

bool Button::isClicked(const sf::Vector2f& mousePos) const {
    return isHovered(mousePos);
}

void Button::setCallback(std::function<void()> cb) {
    callback = cb;
}

void Button::onClick(const sf::Vector2f& mousePos) {
    if (isClicked(mousePos) && callback) {
        callback();
    }
}
