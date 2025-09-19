#include "Button.hpp"

Button::Button(const sf::Vector2f& pos, const sf::Vector2f& size, const std::string& text) {
    shape.setPosition(pos);
    shape.setSize(size);
    shape.setFillColor(sf::Color(200, 200, 200));
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2);

    if (!font.loadFromFile("src/assets/fonts/DejaVuSans-Bold.ttf")) {
        throw std::runtime_error("Impossible de charger la police !");
    }
    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(14);
    label.setFillColor(sf::Color::Black);

    sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
    label.setPosition(pos.x + size.x / 2, pos.y + size.y / 2);
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(label);
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
