#pragma once
#include <SFML/Graphics.hpp>

class Canvas {
public:
    Canvas(int width, int height);

    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape background;
};
