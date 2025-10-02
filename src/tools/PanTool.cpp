#include "PanTool.hpp"
#include "../core/ImageManager.hpp"

PanTool::PanTool() {}

void PanTool::onMousePressed(const sf::Vector2f& pos, Image* /* image */) {
    panning = true;
    lastMousePos = pos;
}

void PanTool::onMouseReleased(const sf::Vector2f& /* pos */, Image* /* image */) {
    panning = false;
    lastMousePos.reset();
}

void PanTool::onMouseMoved(const sf::Vector2f& pos, Image* image) {
    if (!panning || !lastMousePos.has_value() || !image) return;
    
    sf::Vector2f delta = pos - *lastMousePos;
    sf::Vector2f currentPos = image->getViewPosition();
    image->setViewPosition(currentPos + delta);
    
    lastMousePos = pos;
}
