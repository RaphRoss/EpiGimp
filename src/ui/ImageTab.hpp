#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

class ImageTab {
public:
    ImageTab(const std::string& name, size_t imageIndex);
    
    void draw(sf::RenderWindow& window, const sf::Vector2f& position, bool isActive);
    void setModified(bool modified) { isModified = modified; }
    bool getModified() const { return isModified; }
    void setName(const std::string& name) { imageName = name; }
    std::string getName() const { return imageName; }
    size_t getImageIndex() const { return imageIndex; }
    
    sf::FloatRect getBounds() const { return bounds; }
    sf::FloatRect getCloseButtonBounds() const { return closeButtonBounds; }
    
    static const float TAB_WIDTH;
    static const float TAB_HEIGHT;
    
private:
    void updateLayout(const sf::Vector2f& position);
    
    std::string imageName;
    size_t imageIndex;
    bool isModified = false;
    sf::FloatRect bounds;
    sf::FloatRect closeButtonBounds;
    
    sf::RectangleShape tabShape;
    sf::Text tabText;
    sf::CircleShape modifiedIndicator;
    sf::RectangleShape closeButton;
    sf::Text closeButtonText;
    
    static sf::Font font;
    static bool fontLoaded;
};
