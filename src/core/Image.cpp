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
}

void Image::setZoom(float zoom) {
    zoomLevel = std::max(0.1f, std::min(10.0f, zoom));
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
