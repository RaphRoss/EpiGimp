#pragma once
#include <SFML/Graphics.hpp>
#include <deque>

class ColorManager {
public:
    static ColorManager& instance();

    const sf::Color& getForeground() const;
    const sf::Color& getBackground() const;

    void setForeground(const sf::Color& color);
    void setBackground(const sf::Color& color);

    void swapColors();
    void reset();

    const std::deque<sf::Color>& getHistory() const;

private:
    ColorManager();
    void pushHistory(const sf::Color& color);

    sf::Color foreground;
    sf::Color background;
    std::deque<sf::Color> history;
    static constexpr std::size_t HISTORY_LIMIT = 20;
};
