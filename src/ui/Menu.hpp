#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>

struct MenuItem {
    std::string label;
    std::function<void()> callback;
    
    MenuItem(const std::string& text, std::function<void()> cb) 
        : label(text), callback(cb) {}
};

class Menu {
public:
    Menu(const sf::Vector2f& position, const std::string& title);
    
    void addItem(const std::string& label, std::function<void()> callback);
    void draw(sf::RenderWindow& window);
    void handleClick(const sf::Vector2f& mousePos);
    void handleMouseMove(const sf::Vector2f& mousePos);
    
    bool isOpen() const { return open; }
    void setOpen(bool state) { open = state; }
    void toggleOpen() { open = !open; }
    
    sf::FloatRect getTitleBounds() const;
    std::string getTitle() const { return title; }
    
private:
    void updateLayout();
    
    sf::Vector2f position;
    std::string title;
    std::vector<MenuItem> items;
    bool open = false;
    
    sf::RectangleShape titleRect;
    sf::Text titleText;
    std::vector<sf::RectangleShape> itemRects;
    std::vector<sf::Text> itemTexts;
    int hoveredItemIndex = -1;
    
    static sf::Font font;
    static bool fontLoaded;
    
    static const float ITEM_HEIGHT;
    static const float MENU_WIDTH;
};
