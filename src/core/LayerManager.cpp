#include "LayerManager.hpp"
#include <algorithm>
#include <stdexcept>

LayerManager::LayerManager() : activeLayerIndex(0) {}

size_t LayerManager::createLayer(unsigned int width, unsigned int height, const std::string& name) {
    auto layer = std::make_unique<Layer>(width, height, name);
    layers.push_back(std::move(layer));
    size_t index = layers.size() - 1;
    
    if (layers.size() == 1) {
        activeLayerIndex = 0;
    }
    
    notifyLayersChanged();
    return index;
}

size_t LayerManager::createLayerFromImage(const sf::Image& imageData, const std::string& name) {
    auto layer = std::make_unique<Layer>(imageData, name);
    layers.push_back(std::move(layer));
    size_t index = layers.size() - 1;
    
    if (layers.size() == 1) {
        activeLayerIndex = 0;
    }
    
    notifyLayersChanged();
    return index;
}

void LayerManager::deleteLayer(size_t index) {
    if (index >= layers.size()) return;
    if (layers.size() == 1) return; // Can't delete the last layer
    
    layers.erase(layers.begin() + index);
    
    // Adjust active layer index
    if (activeLayerIndex >= layers.size()) {
        activeLayerIndex = layers.size() - 1;
    }
    
    notifyLayersChanged();
}

void LayerManager::duplicateLayer(size_t index) {
    if (index >= layers.size()) return;
    
    Layer* original = layers[index].get();
    sf::Image imageData = original->getImageData();
    std::string newName = original->getName() + " copy";
    
    auto newLayer = std::make_unique<Layer>(imageData, newName);
    newLayer->setOpacity(original->getOpacity());
    newLayer->setBlendMode(original->getBlendMode());
    newLayer->setVisible(original->isVisible());
    
    // Insert right after the original
    layers.insert(layers.begin() + index + 1, std::move(newLayer));
    
    notifyLayersChanged();
}

void LayerManager::moveLayerUp(size_t index) {
    if (index == 0 || index >= layers.size()) return;
    
    std::swap(layers[index], layers[index - 1]);
    
    if (activeLayerIndex == index) {
        activeLayerIndex = index - 1;
    } else if (activeLayerIndex == index - 1) {
        activeLayerIndex = index;
    }
    
    notifyLayersChanged();
}

void LayerManager::moveLayerDown(size_t index) {
    if (index >= layers.size() - 1) return;
    
    std::swap(layers[index], layers[index + 1]);
    
    if (activeLayerIndex == index) {
        activeLayerIndex = index + 1;
    } else if (activeLayerIndex == index + 1) {
        activeLayerIndex = index;
    }
    
    notifyLayersChanged();
}

void LayerManager::mergeDown(size_t index) {
    if (index == 0 || index >= layers.size()) return;
    
    // Get both layers
    Layer* upper = layers[index].get();
    Layer* lower = layers[index - 1].get();
    
    // Create composite
    sf::RenderTexture composite;
    composite.create(lower->getSize().x, lower->getSize().y);
    composite.clear(sf::Color::Transparent);
    
    // Draw lower layer
    composite.draw(lower->getSprite());
    
    // Draw upper layer with blend mode
    sf::Sprite upperSprite = upper->getSprite();
    composite.draw(upperSprite);
    
    composite.display();
    
    // Update lower layer
    lower->setImageData(composite.getTexture().copyToImage());
    
    // Delete upper layer
    deleteLayer(index);
}

void LayerManager::flattenImage() {
    if (layers.empty()) return;
    
    sf::Image composite = getCompositeImage();
    
    // Clear all layers and create a single one
    layers.clear();
    createLayerFromImage(composite, "Background");
    activeLayerIndex = 0;
    
    notifyLayersChanged();
}

void LayerManager::setActiveLayer(size_t index) {
    if (index >= layers.size()) return;
    activeLayerIndex = index;
    notifyLayersChanged();
}

Layer* LayerManager::getActiveLayer() {
    if (activeLayerIndex >= layers.size()) return nullptr;
    return layers[activeLayerIndex].get();
}

const Layer* LayerManager::getActiveLayer() const {
    if (activeLayerIndex >= layers.size()) return nullptr;
    return layers[activeLayerIndex].get();
}

Layer* LayerManager::getLayer(size_t index) {
    if (index >= layers.size()) return nullptr;
    return layers[index].get();
}

const Layer* LayerManager::getLayer(size_t index) const {
    if (index >= layers.size()) return nullptr;
    return layers[index].get();
}

sf::Image LayerManager::getCompositeImage() const {
    if (layers.empty()) {
        sf::Image empty;
        empty.create(1, 1, sf::Color::Transparent);
        return empty;
    }
    
    // Use the first layer's dimensions
    sf::Vector2u size = layers[0]->getSize();
    
    sf::RenderTexture composite;
    composite.create(size.x, size.y);
    composite.clear(sf::Color::Transparent);
    
    // Render from bottom to top
    for (const auto& layer : layers) {
        if (!layer->isVisible()) continue;
        
        sf::Sprite sprite = layer->getSprite();
        composite.draw(sprite);
    }
    
    composite.display();
    return composite.getTexture().copyToImage();
}

void LayerManager::renderComposite(sf::RenderTexture& target, const sf::Vector2f& position) const {
    // Render from bottom to top
    for (const auto& layer : layers) {
        if (!layer->isVisible()) continue;
        
        sf::Sprite sprite = layer->getSprite();
        sprite.setPosition(position);
        target.draw(sprite);
    }
}

void LayerManager::notifyLayersChanged() {
    if (onLayersChanged) {
        onLayersChanged();
    }
}

sf::Color LayerManager::blendColors(const sf::Color& base, const sf::Color& overlay, BlendMode mode, float opacity) const {
    // Simple blend implementation - can be extended with more sophisticated blending
    float alpha = (overlay.a / 255.f) * opacity;
    
    switch (mode) {
        case BlendMode::Normal:
        default: {
            float invAlpha = 1.0f - alpha;
            return sf::Color(
                static_cast<sf::Uint8>(base.r * invAlpha + overlay.r * alpha),
                static_cast<sf::Uint8>(base.g * invAlpha + overlay.g * alpha),
                static_cast<sf::Uint8>(base.b * invAlpha + overlay.b * alpha),
                static_cast<sf::Uint8>(std::min(255.f, base.a + overlay.a * opacity))
            );
        }
        case BlendMode::Multiply: {
            return sf::Color(
                static_cast<sf::Uint8>((base.r * overlay.r) / 255),
                static_cast<sf::Uint8>((base.g * overlay.g) / 255),
                static_cast<sf::Uint8>((base.b * overlay.b) / 255),
                base.a
            );
        }
        case BlendMode::Screen: {
            return sf::Color(
                static_cast<sf::Uint8>(255 - ((255 - base.r) * (255 - overlay.r)) / 255),
                static_cast<sf::Uint8>(255 - ((255 - base.g) * (255 - overlay.g)) / 255),
                static_cast<sf::Uint8>(255 - ((255 - base.b) * (255 - overlay.b)) / 255),
                base.a
            );
        }
        // Add more blend modes as needed
    }
}
