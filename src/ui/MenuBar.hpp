#pragma once
#include "Menu.hpp"
#include <vector>
#include <memory>

class MenuBar {
public:
    MenuBar();
    
    void addMenu(const std::string& title);
    void addMenuItem(const std::string& menuTitle, const std::string& itemLabel, std::function<void()> callback);
    
    void draw(sf::RenderWindow& window);
    bool handleClick(const sf::Vector2f& mousePos); // Retourne true si le clic a été géré
    void handleMouseMove(const sf::Vector2f& mousePos);
    
    static const float MENU_BAR_HEIGHT;
    
private:
    void updateMenuPositions();
    void closeAllMenus();
    
    std::vector<std::unique_ptr<Menu>> menus;
    sf::RectangleShape background;
};
