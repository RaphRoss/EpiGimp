#pragma once
#include "Layer.hpp"
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <SFML/Graphics.hpp>

class LayerManager {
public:
    LayerManager();

    // Layer creation
    size_t createLayer(unsigned int width, unsigned int height, const std::string& name = "Layer");
    size_t createLayerFromImage(const sf::Image& imageData, const std::string& name = "Layer");
    
    // Layer operations
    void deleteLayer(size_t index);
    void duplicateLayer(size_t index);
    void moveLayerUp(size_t index);
    void moveLayerDown(size_t index);
    void mergeDown(size_t index);
    void flattenImage();
    
    // Active layer management
    void setActiveLayer(size_t index);
    size_t getActiveLayerIndex() const { return activeLayerIndex; }
    Layer* getActiveLayer();
    const Layer* getActiveLayer() const;
    
    // Layer access
    Layer* getLayer(size_t index);
    const Layer* getLayer(size_t index) const;
    size_t getLayerCount() const { return layers.size(); }
    
    // Composite rendering
    sf::Image getCompositeImage() const;
    void renderComposite(sf::RenderTexture& target, const sf::Vector2f& position = {0, 0}) const;
    
    // Callbacks for UI updates
    void setOnLayersChanged(std::function<void()> callback) { onLayersChanged = callback; }
    void markLayersModified() { notifyLayersChanged(); }  // Public method to trigger update
    
private:
    std::vector<std::unique_ptr<Layer>> layers;
    size_t activeLayerIndex;
    std::function<void()> onLayersChanged;
    
    void notifyLayersChanged();
    sf::Color blendColors(const sf::Color& base, const sf::Color& overlay, BlendMode mode, float opacity) const;
};
