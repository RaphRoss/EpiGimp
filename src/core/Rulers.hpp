#pragma once
#include <SFML/Graphics.hpp>

class Rulers {
public:
    Rulers();
    
    void draw(sf::RenderWindow& window, const sf::Vector2f& imagePos, const sf::Vector2f& imageSize, 
              float zoom, const sf::Vector2f& mousePos);
    void setVisible(bool visible) { isVisible = visible; }
    bool getVisible() const { return isVisible; }
    void setUnit(const std::string& unit) { currentUnit = unit; }
    
    static const float RULER_SIZE;

private:
    bool isVisible = false;
    std::string currentUnit = "px";
    sf::Color rulerColor = sf::Color(200, 200, 200);
    sf::Color rulerTextColor = sf::Color(0, 0, 0);
    sf::Color crosshairColor = sf::Color(255, 0, 0, 150);
    
    static sf::Font font;
    static bool fontLoaded;
    
    float calculateOptimalStep(float zoom) const;
    std::string formatValue(float value, float step) const;
    
    void drawHorizontalRuler(sf::RenderWindow& window, const sf::Vector2f& imagePos, 
                            const sf::Vector2f& imageSize, float zoom, const sf::Vector2f& mousePos);
    void drawVerticalRuler(sf::RenderWindow& window, const sf::Vector2f& imagePos, 
                          const sf::Vector2f& imageSize, float zoom, const sf::Vector2f& mousePos);
    void drawCrosshair(sf::RenderWindow& window, const sf::Vector2f& mousePos, 
                      const sf::Vector2f& imagePos, const sf::Vector2f& imageSize);
};
