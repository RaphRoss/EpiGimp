#pragma once
#include <SFML/Graphics.hpp>

class Canvas {
public:
    Canvas(int width, int height);

    void draw(sf::RenderWindow& window);
    sf::RenderTexture& getTexture();

private:
    sf::RenderTexture renderTexture;
    sf::Sprite sprite;
};
