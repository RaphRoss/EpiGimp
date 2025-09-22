#pragma once
#include <SFML/Graphics.hpp>

class Canvas {
public:
    Canvas(int width, int height, const sf::Vector2f& position = {200, 30});

    void draw(sf::RenderWindow& window);
    sf::RenderTexture& getTexture();
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
    
    sf::Vector2f getPosition() const { return position; }
    sf::Vector2f getSize() const { return {static_cast<float>(canvasWidth), static_cast<float>(canvasHeight)}; }
    sf::FloatRect getBounds() const;

private:
    sf::RenderTexture renderTexture;
    sf::Sprite sprite;
    sf::Vector2f position;
    int canvasWidth, canvasHeight;
    
    void updateSpritePosition();
};
