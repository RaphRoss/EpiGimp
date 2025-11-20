#pragma once
#include <SFML/Graphics.hpp>
#include "../core/LayerManager.hpp"
#include <vector>
#include <map>
#include <functional>

class LayerPanel {
public:
    LayerPanel(float x, float y, float width, float height);
    
    void setLayerManager(LayerManager* manager);
    void update();
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
        sf::Texture thumbnailTexture;
        sf::Text nameText;
        sf::RectangleShape visibilityButton;
        sf::Text visibilityIcon;
        size_t layerIndex;
        bool isActive;
    };
    
    float x, y, width, height;
    LayerManager* layerManager;
    
    sf::RectangleShape background;
    sf::RectangleShape titleBar;
    sf::Text titleText;
    sf::Font font;
    
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
