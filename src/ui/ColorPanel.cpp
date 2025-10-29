#include "ColorPanel.hpp"

ColorPanel::ColorPanel() {
    fgRect.setSize({40, 40});
    bgRect.setSize({40, 40});
    fgRect.setOutlineColor(sf::Color(63,63,70));
    fgRect.setOutlineThickness(1);
    bgRect.setOutlineColor(sf::Color(63,63,70));
    bgRect.setOutlineThickness(1);
    setPosition({10, 610});
}

void ColorPanel::setPosition(const sf::Vector2f& pos) {
    bgRect.setPosition(pos.x + 24, pos.y + 24);
    fgRect.setPosition(pos);
    bounds = sf::FloatRect(pos.x, pos.y, 64, 64);
}

void ColorPanel::draw(sf::RenderWindow& window) {
    fgRect.setFillColor(ColorManager::instance().getForeground());
    bgRect.setFillColor(ColorManager::instance().getBackground());
    window.draw(bgRect);
    window.draw(fgRect);
}

bool ColorPanel::handleClick(const sf::Vector2f& mousePos, sf::Mouse::Button button) {
    if (!bounds.contains(mousePos)) return false;
    if (!fgRect.getGlobalBounds().contains(mousePos) && !bgRect.getGlobalBounds().contains(mousePos)) return false;
    if (onOpenPicker) onOpenPicker(button == sf::Mouse::Left);
    return true;
}
