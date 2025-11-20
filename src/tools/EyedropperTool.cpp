#include "EyedropperTool.hpp"
#include "../core/ColorManager.hpp"

void EyedropperTool::onMousePressed(const sf::Vector2f& pos, Image* image, sf::Mouse::Button button) {
    (void)button;
    if (!image) return;
    sf::Vector2f ip = image->worldToImage(pos);
    sf::Image data = image->getImageData();
    if (ip.x < 0 || ip.y < 0) return;
    unsigned int x = static_cast<unsigned int>(ip.x);
    unsigned int y = static_cast<unsigned int>(ip.y);
    if (x >= data.getSize().x || y >= data.getSize().y) return;
    sf::Color c = data.getPixel(x, y);
    ColorManager::instance().setForeground(c);
}

void EyedropperTool::onMouseReleased(const sf::Vector2f&, Image* image, sf::Mouse::Button button) {
    (void)image; (void)button;
}

void EyedropperTool::onMouseMoved(const sf::Vector2f&, Image*) {}
