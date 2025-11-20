#include "Layer.hpp"
#include <algorithm>

Layer::Layer(unsigned int width, unsigned int height, const std::string& name)
    : name(name), width(width), height(height), visible(true), opacity(1.0f), blendMode(BlendMode::Normal) {
    if (!renderTexture.create(width, height)) {
        throw std::runtime_error("Failed to create layer render texture");
    }
    renderTexture.clear(sf::Color::Transparent);
    renderTexture.display();
}

Layer::Layer(const sf::Image& imageData, const std::string& name)
    : name(name), visible(true), opacity(1.0f), blendMode(BlendMode::Normal) {
    width = imageData.getSize().x;
    height = imageData.getSize().y;
    
    if (!renderTexture.create(width, height)) {
        throw std::runtime_error("Failed to create layer render texture");
    }
    
    sf::Texture temp;
    temp.loadFromImage(imageData);
    sf::Sprite sprite(temp);
    renderTexture.clear(sf::Color::Transparent);
    renderTexture.draw(sprite);
    renderTexture.display();
}

sf::Image Layer::getImageData() const {
    return renderTexture.getTexture().copyToImage();
}

sf::Sprite Layer::getSprite() const {
    sf::Sprite sprite(renderTexture.getTexture());
    sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(opacity * 255)));
    return sprite;
}

sf::Image Layer::getThumbnail(unsigned int maxWidth, unsigned int maxHeight) const {
    sf::Image fullImage = getImageData();
    
    // Calculate scaling to fit within max dimensions while preserving aspect ratio
    float scaleX = static_cast<float>(maxWidth) / width;
    float scaleY = static_cast<float>(maxHeight) / height;
    float scale = std::min(scaleX, scaleY);
    
    unsigned int thumbWidth = static_cast<unsigned int>(width * scale);
    unsigned int thumbHeight = static_cast<unsigned int>(height * scale);
    
    // Simple nearest-neighbor downscaling
    sf::Image thumbnail;
    thumbnail.create(thumbWidth, thumbHeight);
    
    for (unsigned int y = 0; y < thumbHeight; ++y) {
        for (unsigned int x = 0; x < thumbWidth; ++x) {
            unsigned int srcX = static_cast<unsigned int>(x / scale);
            unsigned int srcY = static_cast<unsigned int>(y / scale);
            srcX = std::min(srcX, width - 1);
            srcY = std::min(srcY, height - 1);
            thumbnail.setPixel(x, y, fullImage.getPixel(srcX, srcY));
        }
    }
    
    return thumbnail;
}

void Layer::setImageData(const sf::Image& imageData) {
    sf::Texture temp;
    temp.loadFromImage(imageData);
    sf::Sprite sprite(temp);
    renderTexture.clear(sf::Color::Transparent);
    renderTexture.draw(sprite);
    renderTexture.display();
}

void Layer::clear(const sf::Color& color) {
    renderTexture.clear(color);
}

void Layer::draw(const sf::Drawable& drawable, const sf::RenderStates& states) {
    renderTexture.draw(drawable, states);
}

void Layer::display() {
    renderTexture.display();
}
