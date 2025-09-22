#pragma once
#include <SFML/Graphics.hpp>
#include "../tools/Tool.hpp"
#include <vector>
#include <memory>
#include <functional>

struct ToolButton {
    sf::RectangleShape shape;
    sf::Text label;
    std::string toolName;
    std::function<void()> callback;
    bool selected = false;
};

class ToolPanel {
public:
    ToolPanel();
    
    void addTool(const std::string& name, const std::string& displayName, std::function<void()> callback);
    void draw(sf::RenderWindow& window);
    void handleClick(const sf::Vector2f& mousePos);
    
    void setSelectedTool(const std::string& toolName);
    
    static const float PANEL_WIDTH;
    static const float BUTTON_HEIGHT;
    
private:
    void updateLayout();
    
    std::vector<ToolButton> toolButtons;
    sf::RectangleShape background;
    
    static sf::Font font;
    static bool fontLoaded;
};
