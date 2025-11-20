#pragma once
#include <SFML/Graphics.hpp>
#include "../core/LayerManager.hpp"
#include <vector>
#include <map>
#include <functional>
#include <memory>

class LayerPanel {
public:
    LayerPanel(float x, float y, float width, float height);
    
    void setLayerManager(LayerManager* manager);
    void update();
    void refresh() { rebuildLayerList(); }  // Public method to force refresh
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    
    bool contains(const sf::Vector2f& point) const;
    
    // Callbacks
    void setOnLayerSelected(std::function<void(size_t)> callback) { onLayerSelected = callback; }
    void setOnLayerVisibilityToggled(std::function<void(size_t)> callback) { onLayerVisibilityToggled = callback; }

private:
    struct LayerItem {
        sf::RectangleShape background;
        sf::RectangleShape thumbnail;
        std::shared_ptr<sf::Texture> thumbnailTexture;  // Use shared_ptr to avoid dangling pointers
        sf::Text nameText;
        sf::RectangleShape visibilityButton;
        sf::Text visibilityIcon;
        size_t layerIndex;
        bool isActive;
    };
    
    float x, y, width, height;
    LayerManager* layerManager;
    
    // Font must be declared BEFORE any sf::Text that uses it!
    sf::Font font;
    
    sf::RectangleShape background;
    sf::RectangleShape titleBar;
    sf::Text titleText;
    
    std::vector<LayerItem> layerItems;
    float scrollOffset;
    float itemHeight;
    
    // Buttons
    sf::RectangleShape newLayerButton;
    sf::Text newLayerButtonText;
    sf::RectangleShape deleteLayerButton;
    sf::Text deleteLayerButtonText;
    sf::RectangleShape duplicateLayerButton;
    sf::Text duplicateLayerButtonText;
    
    std::function<void(size_t)> onLayerSelected;
    std::function<void(size_t)> onLayerVisibilityToggled;
    
    void rebuildLayerList();
    void updateThumbnail(LayerItem& item, const Layer* layer);
    sf::Vector2f getMousePosition(const sf::RenderWindow& window) const;
};
