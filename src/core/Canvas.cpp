#include "Canvas.hpp"
#include <iostream>

Canvas::Canvas(int width, int height, const sf::Vector2f& pos) 
    : position(pos), canvasWidth(width - 200), canvasHeight(height - 30) {
    renderTexture.create(canvasWidth, canvasHeight);
    renderTexture.clear(sf::Color::White);
    renderTexture.display();

    sprite.setTexture(renderTexture.getTexture());
    updateSpritePosition();
}

void Canvas::updateSpritePosition() {
    sprite.setPosition(position);
}

void Canvas::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::RenderTexture& Canvas::getTexture() {
    return renderTexture;
}

void Canvas::saveToFile(const std::string& filename) {
    sf::Texture tex = renderTexture.getTexture();
    sf::Image screenshot = tex.copyToImage();
    if (screenshot.saveToFile(filename)) {
        std::cout << "Image sauvegardée : " << filename << std::endl;
    } else {
        std::cerr << "Erreur lors de la sauvegarde de " << filename << std::endl;
    }
}

void Canvas::loadFromFile(const std::string& filename) {
    sf::Image image;
    if (image.loadFromFile(filename)) {
        sf::Texture tempTexture;
        tempTexture.loadFromImage(image);
        
        renderTexture.clear(sf::Color::White);
        
        sf::Sprite loadedSprite(tempTexture);
        sf::Vector2u imageSize = image.getSize();
        if (imageSize.x > static_cast<unsigned int>(canvasWidth) || imageSize.y > static_cast<unsigned int>(canvasHeight)) {
            float scaleX = static_cast<float>(canvasWidth) / imageSize.x;
            float scaleY = static_cast<float>(canvasHeight) / imageSize.y;
            float scale = std::min(scaleX, scaleY);
            loadedSprite.setScale(scale, scale);
        }
        
        renderTexture.draw(loadedSprite);
        renderTexture.display();
        
        std::cout << "Image chargée : " << filename << std::endl;
    } else {
        std::cerr << "Erreur lors du chargement de " << filename << std::endl;
    }
}

sf::FloatRect Canvas::getBounds() const {
    return sf::FloatRect(position.x, position.y, static_cast<float>(canvasWidth), static_cast<float>(canvasHeight));
}
