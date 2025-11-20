#include "LayerPanel.hpp"
#include <iostream>

LayerPanel::LayerPanel(float x, float y, float width, float height)
    : x(x), y(y), width(width), height(height), layerManager(nullptr), scrollOffset(0), itemHeight(60) {
    
    // Background
    background.setSize({width, height});
    background.setPosition(x, y);
    background.setFillColor(sf::Color(50, 50, 50));
    background.setOutlineColor(sf::Color(30, 30, 30));
    background.setOutlineThickness(2);
    
    // Title bar
    titleBar.setSize({width, 30});
    titleBar.setPosition(x, y);
    titleBar.setFillColor(sf::Color(40, 40, 40));
    
    // Load font (try system font)
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Warning: Could not load font for LayerPanel\n";
    }
    
    titleText.setFont(font);
    titleText.setString("Layers");
    titleText.setCharacterSize(14);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(x + 10, y + 5);
    
    // Buttons at bottom
    float buttonY = y + height - 35;
    float buttonWidth = (width - 40) / 3;
    
    newLayerButton.setSize({buttonWidth, 25});
    newLayerButton.setPosition(x + 10, buttonY);
    newLayerButton.setFillColor(sf::Color(70, 70, 70));
    
    newLayerButtonText.setFont(font);
    newLayerButtonText.setString("+");
    newLayerButtonText.setCharacterSize(18);
    newLayerButtonText.setFillColor(sf::Color::White);
    newLayerButtonText.setPosition(x + 10 + buttonWidth/2 - 5, buttonY + 2);
    
    duplicateLayerButton.setSize({buttonWidth, 25});
    duplicateLayerButton.setPosition(x + 15 + buttonWidth, buttonY);
    duplicateLayerButton.setFillColor(sf::Color(70, 70, 70));
    
    duplicateLayerButtonText.setFont(font);
    duplicateLayerButtonText.setString("D");
    duplicateLayerButtonText.setCharacterSize(14);
    duplicateLayerButtonText.setFillColor(sf::Color::White);
    duplicateLayerButtonText.setPosition(x + 15 + buttonWidth + buttonWidth/2 - 5, buttonY + 5);
    
    deleteLayerButton.setSize({buttonWidth, 25});
    deleteLayerButton.setPosition(x + 20 + 2*buttonWidth, buttonY);
    deleteLayerButton.setFillColor(sf::Color(70, 70, 70));
    
    deleteLayerButtonText.setFont(font);
    deleteLayerButtonText.setString("-");
    deleteLayerButtonText.setCharacterSize(18);
    deleteLayerButtonText.setFillColor(sf::Color::White);
    deleteLayerButtonText.setPosition(x + 20 + 2*buttonWidth + buttonWidth/2 - 5, buttonY + 2);
}

void LayerPanel::setLayerManager(LayerManager* manager) {
    layerManager = manager;
    rebuildLayerList();
}

void LayerPanel::update() {
    if (!layerManager) return;
    rebuildLayerList();
}

void LayerPanel::rebuildLayerList() {
    if (!layerManager) return;
    
    layerItems.clear();
    
    size_t layerCount = layerManager->getLayerCount();
    float currentY = y + 35 + scrollOffset;
    
    // Build items from top layer to bottom (reverse order for display)
    for (size_t i = 0; i < layerCount; ++i) {
        size_t displayIndex = layerCount - 1 - i;
        const Layer* layer = layerManager->getLayer(displayIndex);
        if (!layer) continue;
        
        LayerItem item;
        item.layerIndex = displayIndex;
        item.isActive = (displayIndex == layerManager->getActiveLayerIndex());
        
        // Background
        item.background.setSize({width - 20, itemHeight - 5});
        item.background.setPosition(x + 10, currentY);
        item.background.setFillColor(item.isActive ? sf::Color(80, 120, 180) : sf::Color(60, 60, 60));
        item.background.setOutlineColor(sf::Color(40, 40, 40));
        item.background.setOutlineThickness(1);
        
        // Thumbnail
        item.thumbnail.setSize({50, 50});
        item.thumbnail.setPosition(x + 15, currentY + 5);
        item.thumbnail.setFillColor(sf::Color::White);
        updateThumbnail(item, layer);
        
        // Name text
        item.nameText.setFont(font);
        item.nameText.setString(layer->getName());
        item.nameText.setCharacterSize(12);
        item.nameText.setFillColor(sf::Color::White);
        item.nameText.setPosition(x + 75, currentY + 10);
        
        // Visibility button (eye icon)
        item.visibilityButton.setSize({30, 30});
        item.visibilityButton.setPosition(x + width - 45, currentY + 15);
        item.visibilityButton.setFillColor(sf::Color(50, 50, 50));
        
        item.visibilityIcon.setFont(font);
        item.visibilityIcon.setString(layer->isVisible() ? "O" : "X");
        item.visibilityIcon.setCharacterSize(14);
        item.visibilityIcon.setFillColor(layer->isVisible() ? sf::Color::Green : sf::Color::Red);
        item.visibilityIcon.setPosition(x + width - 37, currentY + 20);
        
        layerItems.push_back(item);
        currentY += itemHeight;
    }
}

void LayerPanel::updateThumbnail(LayerItem& item, const Layer* layer) {
    sf::Image thumbImage = layer->getThumbnail(50, 50);
    item.thumbnailTexture.loadFromImage(thumbImage);
    item.thumbnail.setTexture(&item.thumbnailTexture);
}

void LayerPanel::draw(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(titleBar);
    window.draw(titleText);
    
    // Draw layer items with clipping
    sf::View originalView = window.getView();
    sf::View clippedView(sf::FloatRect(x, y + 35, width, height - 75));
    clippedView.setViewport(sf::FloatRect(
        x / window.getSize().x,
        (y + 35) / window.getSize().y,
        width / window.getSize().x,
        (height - 75) / window.getSize().y
    ));
    
    for (const auto& item : layerItems) {
        // Check if item is within visible area
        float itemY = item.background.getPosition().y;
        if (itemY + itemHeight < y + 35 || itemY > y + height - 40) continue;
        
        window.draw(item.background);
        window.draw(item.thumbnail);
        window.draw(item.nameText);
        window.draw(item.visibilityButton);
        window.draw(item.visibilityIcon);
    }
    
    // Draw buttons
    window.draw(newLayerButton);
    window.draw(newLayerButtonText);
    window.draw(duplicateLayerButton);
    window.draw(duplicateLayerButtonText);
    window.draw(deleteLayerButton);
    window.draw(deleteLayerButtonText);
}

void LayerPanel::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (!layerManager) return;
    
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = getMousePosition(window);
        
        // Check button clicks
        if (newLayerButton.getGlobalBounds().contains(mousePos)) {
            if (layerManager->getLayerCount() > 0) {
                const Layer* firstLayer = layerManager->getLayer(0);
                if (firstLayer) {
                    layerManager->createLayer(firstLayer->getSize().x, firstLayer->getSize().y, "New Layer");
                }
            }
            return;
        }
        
        if (duplicateLayerButton.getGlobalBounds().contains(mousePos)) {
            layerManager->duplicateLayer(layerManager->getActiveLayerIndex());
            return;
        }
        
        if (deleteLayerButton.getGlobalBounds().contains(mousePos)) {
            if (layerManager->getLayerCount() > 1) {
                layerManager->deleteLayer(layerManager->getActiveLayerIndex());
            }
            return;
        }
        
        // Check layer item clicks
        for (const auto& item : layerItems) {
            if (item.visibilityButton.getGlobalBounds().contains(mousePos)) {
                Layer* layer = layerManager->getLayer(item.layerIndex);
                if (layer) {
                    layer->setVisible(!layer->isVisible());
                    if (onLayerVisibilityToggled) {
                        onLayerVisibilityToggled(item.layerIndex);
                    }
                }
                return;
            }
            
            if (item.background.getGlobalBounds().contains(mousePos)) {
                layerManager->setActiveLayer(item.layerIndex);
                if (onLayerSelected) {
                    onLayerSelected(item.layerIndex);
                }
                return;
            }
        }
    }
    
    // Scroll support
    if (event.type == sf::Event::MouseWheelScrolled) {
        sf::Vector2f mousePos = getMousePosition(window);
        if (contains(mousePos)) {
            scrollOffset += event.mouseWheelScroll.delta * 20;
            scrollOffset = std::max(scrollOffset, -(static_cast<float>(layerItems.size()) * itemHeight - (height - 75)));
            scrollOffset = std::min(scrollOffset, 0.f);
        }
    }
}

bool LayerPanel::contains(const sf::Vector2f& point) const {
    return background.getGlobalBounds().contains(point);
}

sf::Vector2f LayerPanel::getMousePosition(const sf::RenderWindow& window) const {
    return window.mapPixelToCoords(sf::Mouse::getPosition(window));
}
