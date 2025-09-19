#include "Canvas.hpp"

Canvas::Canvas(int width, int height) {
    renderTexture.create(width, height);
    renderTexture.clear(sf::Color::White); // Feuille blanche
    renderTexture.display();

    sprite.setTexture(renderTexture.getTexture());
    sprite.setPosition(0, 0);
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
    screenshot.saveToFile(filename);
}
