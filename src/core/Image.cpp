#include "Image.hpp"
#include "Command.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>

Image::Image(int width, int height, const std::string& name) 
    : originalSize(width, height), imageName(name) {
    setupTexture();
}

Image::Image(const std::string& filepath) {
    sf::Image loadedImage;
    if (loadedImage.loadFromFile(filepath)) {
        originalSize = sf::Vector2i(loadedImage.getSize());
        imageName = filepath.substr(filepath.find_last_of("/\\") + 1);
        setupTexture();
        sf::Texture tempTexture;
        tempTexture.loadFromImage(loadedImage);
        sf::Sprite tempSprite(tempTexture);
        renderTexture.clear(sf::Color::Transparent);
        renderTexture.draw(tempSprite);
        renderTexture.display();
        updateSprite();
        std::cout << "Image chargée : " << filepath << std::endl;
    } else {
        std::cerr << "Erreur lors du chargement de " << filepath << std::endl;
        originalSize = sf::Vector2i(800, 600);
        imageName = "Error_Image";
        setupTexture();
    }
}

void Image::setupTexture() {
    renderTexture.create(originalSize.x, originalSize.y);
    renderTexture.clear(sf::Color::White);
    renderTexture.display();
    updateSprite();
}

void Image::updateSprite() {
    sprite.setTexture(renderTexture.getTexture());
    sprite.setScale(zoomLevel, zoomLevel);
    sprite.setPosition(viewPosition);
}

sf::Image Image::clipboard;

void Image::draw(sf::RenderWindow& window, const sf::Vector2f& position) {
    viewPosition = position;
    updateSprite();
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
    return renderTexture;
}

sf::Vector2f Image::getDisplaySize() const {
    return sf::Vector2f(originalSize.x * zoomLevel, originalSize.y * zoomLevel);
}

void Image::saveToFile(const std::string& filename) {
    sf::Texture tex = renderTexture.getTexture();
    sf::Image screenshot = tex.copyToImage();
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
    sf::Image currentContent = renderTexture.getTexture().copyToImage();
    
    originalSize = sf::Vector2i(newWidth, newHeight);
    renderTexture.create(newWidth, newHeight);
    renderTexture.clear(sf::Color::Transparent);
    
    sf::Texture tempTexture;
    tempTexture.loadFromImage(currentContent);
    sf::Sprite tempSprite(tempTexture);
    renderTexture.draw(tempSprite);
    renderTexture.display();
    
    updateSprite();
}

void Image::setImageContent(const sf::Image& newContent) {
    sf::Vector2u newSize = newContent.getSize();
    originalSize = sf::Vector2i(newSize.x, newSize.y);
    
    renderTexture.create(newSize.x, newSize.y);
    renderTexture.clear(sf::Color::Transparent);
    
    sf::Texture tempTexture;
    tempTexture.loadFromImage(newContent);
    sf::Sprite tempSprite(tempTexture);
    renderTexture.draw(tempSprite);
    renderTexture.display();
    
    updateSprite();
}

sf::Image Image::getImageData() const {
    return renderTexture.getTexture().copyToImage();
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
