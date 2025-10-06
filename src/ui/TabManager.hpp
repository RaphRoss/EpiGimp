#pragma once
#include <SFML/Graphics.hpp>
#include "ImageTab.hpp"
#include <vector>
#include <memory>
#include <functional>

class TabManager {
public:
    TabManager();
    
    void addTab(const std::string& imageName, size_t imageIndex);
    void removeTab(size_t tabIndex);
    void setActiveTab(size_t tabIndex);
    void setTabModified(size_t imageIndex, bool modified);
    void updateTabName(size_t imageIndex, const std::string& newName);
    
    void draw(sf::RenderWindow& window);
    bool handleClick(const sf::Vector2f& mousePos);
    void handleMouseMove(const sf::Vector2f& mousePos);
    
    size_t getActiveTabIndex() const { return activeTabIndex; }
    size_t getTabCount() const { return tabs.size(); }
    bool hasModifiedTabs() const;
    
    // Callbacks
    void setOnTabSelected(std::function<void(size_t)> callback) { onTabSelected = callback; }
    void setOnTabCloseRequested(std::function<void(size_t)> callback) { onTabCloseRequested = callback; }
    
    static const float TAB_BAR_HEIGHT;
    
private:
    void updateTabLayout();
    size_t findTabByImageIndex(size_t imageIndex);
    
    std::vector<std::unique_ptr<ImageTab>> tabs;
    size_t activeTabIndex = 0;
    sf::RectangleShape background;
    
    std::function<void(size_t)> onTabSelected;
    std::function<void(size_t)> onTabCloseRequested;
    
    // Pour gérer le survol
    int hoveredTabIndex = -1;
    bool hoveredCloseButton = false;
};
