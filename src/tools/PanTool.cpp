#include "PanTool.hpp"
#include "../core/ImageManager.hpp"

PanTool::PanTool() {}

void PanTool::onMousePressed(const sf::Vector2f& pos, Image* image, sf::Mouse::Button button) {
    (void)button;
    panning = true;
    lastMousePos = pos;
    movingFloating = false;
    if (image) {
        sf::Vector2f imagePos = image->worldToImage(pos);
        if (image->hasFloatingSelection()) {
            if (image->isPointInFloating(imagePos)) {
                movingFloating = true;
            }
        } else if (!image->getSelection().isEmpty() && image->getSelection().contains(imagePos)) {
            image->beginFloatingFromSelection(true);
            movingFloating = true;
        }
    }
}

void PanTool::onMouseReleased(const sf::Vector2f& /* pos */, Image* image, sf::Mouse::Button button) {
    (void)button;
    panning = false;
    lastMousePos.reset();
    if (image && movingFloating && image->hasFloatingSelection()) {
        image->commitFloating();
    }
    movingFloating = false;
}

void PanTool::onMouseMoved(const sf::Vector2f& pos, Image* image) {
    if (!panning || !lastMousePos.has_value() || !image) return;
    
    sf::Vector2f delta = pos - *lastMousePos;
    if (image->hasFloatingSelection()) {
        sf::Vector2f deltaImg(delta.x / image->getZoom(), delta.y / image->getZoom());
        image->moveFloatingBy(deltaImg);
    } else {
        sf::Vector2f currentPos = image->getViewPosition();
        image->setViewPosition(currentPos + delta);
    }
    
    lastMousePos = pos;
}
