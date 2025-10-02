#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class StatusBar {
public:
    StatusBar(float windowWidth);
    
    void draw(sf::RenderWindow& window);
    void updateMousePosition(const sf::Vector2f& imagePos, float zoom);
    void updateZoomLevel(float zoom);
    void updateImageInfo(const std::string& imageName, int width, int height);
    void updateToolInfo(const std::string& toolName);
    
    static const float STATUS_BAR_HEIGHT;

private:
    sf::RectangleShape background;
    sf::Text mouseText;
    sf::Text zoomText;
    sf::Text imageText;
    sf::Text toolText;
    
    static sf::Font font;
    static bool fontLoaded;
    
    void updateLayout();
};
