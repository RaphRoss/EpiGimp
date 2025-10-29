#include "ColorManager.hpp"

ColorManager& ColorManager::instance() {
    static ColorManager inst;
    return inst;
}

ColorManager::ColorManager() {
    foreground = sf::Color::Black;
    background = sf::Color::White;
    history.clear();
}

const sf::Color& ColorManager::getForeground() const {
    return foreground;
}

const sf::Color& ColorManager::getBackground() const {
    return background;
}

void ColorManager::setForeground(const sf::Color& color) {
    foreground = color;
    pushHistory(color);
}

void ColorManager::setBackground(const sf::Color& color) {
    background = color;
}

void ColorManager::swapColors() {
    sf::Color tmp = foreground;
    foreground = background;
    background = tmp;
    pushHistory(foreground);
}

void ColorManager::reset() {
    foreground = sf::Color::Black;
    background = sf::Color::White;
    pushHistory(foreground);
}

const std::deque<sf::Color>& ColorManager::getHistory() const {
    return history;
}

void ColorManager::pushHistory(const sf::Color& color) {
    if (!history.empty()) {
        const sf::Color& last = history.front();
        if (last.r == color.r && last.g == color.g && last.b == color.b && last.a == color.a) {
            return;
        }
    }
    history.push_front(color);
    while (history.size() > HISTORY_LIMIT) {
        history.pop_back();
    }
}
