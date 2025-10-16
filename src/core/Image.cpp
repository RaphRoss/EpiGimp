#include "Image.hpp"
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

void Image::draw(sf::RenderWindow& window, const sf::Vector2f& position) {
    viewPosition = position;
    updateSprite();
    window.draw(sprite);
    
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
