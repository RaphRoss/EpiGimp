#include "Image.hpp"
#include "Command.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>

Image::Image(int width, int height, const std::string& name) 
    : originalSize(width, height), imageName(name) {
    // Create initial background layer
    layerManager.createLayer(width, height, "Background");
    
    // Fill background with white
    Layer* bg = layerManager.getActiveLayer();
    if (bg) {
        bg->clear(sf::Color::White);
        bg->display();
    }
    
    updateSprite();
}

Image::Image(const std::string& filepath) {
    sf::Image loadedImage;
    if (loadedImage.loadFromFile(filepath)) {
        originalSize = sf::Vector2i(loadedImage.getSize());
        imageName = filepath.substr(filepath.find_last_of("/\\") + 1);
        
        // Create layer from loaded image
        layerManager.createLayerFromImage(loadedImage, "Background");
        
        updateSprite();
        std::cout << "Image chargée : " << filepath << std::endl;
    } else {
        std::cerr << "Erreur lors du chargement de " << filepath << std::endl;
        originalSize = sf::Vector2i(800, 600);
        imageName = "Error_Image";
        
        // Create empty layer
        layerManager.createLayer(800, 600, "Background");
        
        updateSprite();
    }
}

// Static members initialization
sf::RenderTexture Image::sharedCompositeBuffer;
bool Image::sharedCompositeInitialized = false;

void Image::ensureSharedComposite(unsigned int width, unsigned int height) {
    if (!sharedCompositeInitialized || 
        sharedCompositeBuffer.getSize().x < width ||
        sharedCompositeBuffer.getSize().y < height) {
        // Create or resize to accommodate the largest image
        unsigned int newWidth = std::max(sharedCompositeBuffer.getSize().x, width);
        unsigned int newHeight = std::max(sharedCompositeBuffer.getSize().y, height);
        sharedCompositeBuffer.create(newWidth, newHeight);
        sharedCompositeInitialized = true;
    }
}

void Image::updateSprite() {
    // Sprite will be updated in draw() using composite render
    // This function is kept for compatibility but does minimal work
    sprite.setScale(zoomLevel, zoomLevel);
    sprite.setPosition(viewPosition);
}

sf::Image Image::clipboard;

void Image::draw(sf::RenderWindow& window, const sf::Vector2f& position) {
    viewPosition = position;
    
    // Render composite of all layers on demand using shared buffer
    if (layerManager.getLayerCount() > 0) {
        ensureSharedComposite(originalSize.x, originalSize.y);
        sharedCompositeBuffer.clear(sf::Color::Transparent);
        layerManager.renderComposite(sharedCompositeBuffer);
        sharedCompositeBuffer.display();
        
        sprite.setTexture(sharedCompositeBuffer.getTexture());
        sprite.setScale(zoomLevel, zoomLevel);
        sprite.setPosition(viewPosition);
    }
    
    window.draw(sprite);
    
    if (floatingActive && floatingCut && floatingSrcRect.width > 0 && floatingSrcRect.height > 0) {
        sf::RectangleShape whiteRect;
        whiteRect.setFillColor(sf::Color::White);
        sf::Vector2f topLeft = imageToWorld({static_cast<float>(floatingSrcRect.left), static_cast<float>(floatingSrcRect.top)});
        whiteRect.setPosition(topLeft);
        whiteRect.setSize({floatingSrcRect.width * zoomLevel, floatingSrcRect.height * zoomLevel});
        window.draw(whiteRect);
    }
    
    if (floatingActive && floatingImage.getSize().x > 0 && floatingImage.getSize().y > 0) {
        sf::Texture tex;
        tex.loadFromImage(floatingImage);
        sf::Sprite spr(tex);
        sf::Vector2f worldPos = imageToWorld(floatingOffset);
        spr.setPosition(worldPos);
        spr.setScale(zoomLevel, zoomLevel);
        window.draw(spr);
    }
    
    selection.draw(window, viewPosition, zoomLevel);
}

void Image::setZoom(float zoom) {
    zoomLevel = std::max(0.1f, std::min(10.0f, zoom));
    updateSprite();
}

void Image::setZoomAt(float zoom, const sf::Vector2f& centerPoint) {
    sf::Vector2f imageCenter = worldToImage(centerPoint);
    setZoom(zoom);
    
    // Adjust position to keep the center point stable
    sf::Vector2f newWorldCenter = imageToWorld(imageCenter);
    sf::Vector2f offset = centerPoint - newWorldCenter;
    viewPosition += offset;
    updateSprite();
}

void Image::zoomIn() {
    float newZoom = zoomLevel * 1.25f;
    setZoom(newZoom);
}

void Image::zoomOut() {
    float newZoom = zoomLevel * 0.8f;
    setZoom(newZoom);
}

void Image::zoomInAt(const sf::Vector2f& centerPoint) {
    float newZoom = zoomLevel * 1.25f;
    setZoomAt(newZoom, centerPoint);
}

void Image::zoomOutAt(const sf::Vector2f& centerPoint) {
    float newZoom = zoomLevel * 0.8f;
    setZoomAt(newZoom, centerPoint);
}

void Image::resetZoom() {
    setZoom(1.0f);
}

sf::RenderTexture& Image::getTexture() {
    // Return the active layer's texture for drawing tools
    Layer* activeLayer = layerManager.getActiveLayer();
    if (!activeLayer) {
        throw std::runtime_error("No active layer available for drawing");
    }
    return activeLayer->getTexture();
}

sf::Vector2f Image::getDisplaySize() const {
    return sf::Vector2f(originalSize.x * zoomLevel, originalSize.y * zoomLevel);
}

void Image::saveToFile(const std::string& filename) {
    sf::Image screenshot = layerManager.getCompositeImage();
    if (screenshot.saveToFile(filename)) {
        std::cout << "Image sauvegardée : " << filename << std::endl;
        filePath = filename;
        markAsSaved();
    } else {
        std::cerr << "Erreur lors de la sauvegarde de " << filename << std::endl;
    }
}

sf::FloatRect Image::getBounds() const {
    sf::Vector2f displaySize = getDisplaySize();
    return sf::FloatRect(viewPosition.x, viewPosition.y, displaySize.x, displaySize.y);
}

sf::Vector2f Image::worldToImage(const sf::Vector2f& worldPos) const {
    sf::Vector2f relativePos = worldPos - viewPosition;
    return sf::Vector2f(relativePos.x / zoomLevel, relativePos.y / zoomLevel);
}

sf::Vector2f Image::imageToWorld(const sf::Vector2f& imagePos) const {
    return sf::Vector2f(imagePos.x * zoomLevel + viewPosition.x, 
                       imagePos.y * zoomLevel + viewPosition.y);
}

void Image::resize(unsigned int newWidth, unsigned int newHeight) {
    // Get current composite
    sf::Image currentContent = layerManager.getCompositeImage();
    
    originalSize = sf::Vector2i(newWidth, newHeight);
    // Shared composite will auto-resize on next draw
    
    // For now, just flatten to one layer on resize
    // TODO: resize all layers individually
    layerManager.flattenImage();
    
    updateSprite();
}

void Image::setImageContent(const sf::Image& newContent) {
    sf::Vector2u newSize = newContent.getSize();
    originalSize = sf::Vector2i(newSize.x, newSize.y);
    // Shared composite will auto-resize on next draw
    
    // Update the active layer with new content
    Layer* activeLayer = layerManager.getActiveLayer();
    if (activeLayer) {
        activeLayer->setImageData(newContent);
    }
    
    updateSprite();
}

sf::Image Image::getImageData() const {
    // Return composite of all layers
    return layerManager.getCompositeImage();
}

bool Image::copySelectionToClipboard() {
    if (selection.isEmpty()) return false;
    sf::FloatRect bounds = selection.getBounds();
    sf::IntRect rect(static_cast<int>(bounds.left), static_cast<int>(bounds.top),
                     static_cast<int>(bounds.width), static_cast<int>(bounds.height));
    sf::Image src = getImageData();
    if (rect.width <= 0 || rect.height <= 0) return false;
    clipboard.create(rect.width, rect.height, sf::Color::Transparent);
    for (int y = 0; y < rect.height; ++y) {
        for (int x = 0; x < rect.width; ++x) {
            int sx = rect.left + x;
            int sy = rect.top + y;
            if (sx >= 0 && sy >= 0 && sx < static_cast<int>(src.getSize().x) && sy < static_cast<int>(src.getSize().y)) {
                clipboard.setPixel(x, y, src.getPixel(sx, sy));
            }
        }
    }
    return true;
}

bool Image::pasteClipboardAsFloating() {
    if (clipboard.getSize().x == 0 || clipboard.getSize().y == 0) return false;
    floatingImage = clipboard;
    floatingOffset = {0, 0};
    floatingSrcRect = sf::IntRect(0, 0, static_cast<int>(floatingImage.getSize().x), static_cast<int>(floatingImage.getSize().y));
    floatingActive = true;
    floatingCut = false;
    return true;
}

void Image::beginFloatingFromSelection(bool cutSource) {
    if (selection.isEmpty()) return;
    sf::FloatRect bounds = selection.getBounds();
    sf::IntRect rect(static_cast<int>(bounds.left), static_cast<int>(bounds.top),
                     static_cast<int>(bounds.width), static_cast<int>(bounds.height));
    sf::Image src = getImageData();
    if (rect.width <= 0 || rect.height <= 0) return;
    floatingImage.create(rect.width, rect.height, sf::Color::Transparent);
    for (int y = 0; y < rect.height; ++y) {
        for (int x = 0; x < rect.width; ++x) {
            int sx = rect.left + x;
            int sy = rect.top + y;
            if (sx >= 0 && sy >= 0 && sx < static_cast<int>(src.getSize().x) && sy < static_cast<int>(src.getSize().y)) {
                floatingImage.setPixel(x, y, src.getPixel(sx, sy));
            }
        }
    }
    floatingOffset = {static_cast<float>(rect.left), static_cast<float>(rect.top)};
    floatingSrcRect = rect;
    floatingActive = true;
    floatingCut = cutSource;
}

void Image::beginFloatingFromClipboard() {
    pasteClipboardAsFloating();
}

void Image::moveFloatingBy(const sf::Vector2f& deltaImageSpace) {
    if (!floatingActive) return;
    floatingOffset += deltaImageSpace;
}

void Image::commitFloating() {
    if (!floatingActive) return;
    sf::Image before = getImageData();
    sf::Image after = before;
    if (floatingCut && (floatingSrcRect.width > 0 && floatingSrcRect.height > 0)) {
        for (int y = 0; y < floatingSrcRect.height; ++y) {
            for (int x = 0; x < floatingSrcRect.width; ++x) {
                int dx = floatingSrcRect.left + x;
                int dy = floatingSrcRect.top + y;
                if (dx >= 0 && dy >= 0 && dx < static_cast<int>(after.getSize().x) && dy < static_cast<int>(after.getSize().y)) {
                    after.setPixel(dx, dy, sf::Color::White);
                }
            }
        }
    }
    
    for (unsigned int y = 0; y < floatingImage.getSize().y; ++y) {
        for (unsigned int x = 0; x < floatingImage.getSize().x; ++x) {
            unsigned int dx = static_cast<unsigned int>(std::lround(floatingOffset.x)) + x;
            unsigned int dy = static_cast<unsigned int>(std::lround(floatingOffset.y)) + y;
            if (dx < after.getSize().x && dy < after.getSize().y) {
                after.setPixel(dx, dy, floatingImage.getPixel(x, y));
            }
        }
    }
    HistoryManager& hm = getHistoryManager();
    auto cmd = std::make_unique<ApplyImageCommand>(this, before, after);
    hm.executeCommand(std::move(cmd));
    floatingActive = false;
    selection.clear();
}

bool Image::isPointInFloating(const sf::Vector2f& imagePoint) const {
    if (!floatingActive) return false;
    sf::FloatRect fr(floatingOffset.x, floatingOffset.y,
                     static_cast<float>(floatingImage.getSize().x),
                     static_cast<float>(floatingImage.getSize().y));
    return fr.contains(imagePoint);
}

void Image::cancelFloating() {
    if (!floatingActive) return;
    floatingActive = false;
}
