#include "SelectTool.hpp"
#include "../core/Image.hpp"
#include <algorithm>

SelectTool::SelectTool() {
}

void SelectTool::onMousePressed(const sf::Vector2f& pos, Image* image) {
    if (!image) return;
    
    isSelecting = true;
    startPos = image->worldToImage(pos);
    currentPos = startPos;
}

void SelectTool::onMouseReleased(const sf::Vector2f& pos, Image* image) {
    if (!image || !isSelecting) return;
    
    isSelecting = false;
    currentPos = image->worldToImage(pos);
    
    float minX = std::min(startPos.x, currentPos.x);
    float minY = std::min(startPos.y, currentPos.y);
    float maxX = std::max(startPos.x, currentPos.x);
    float maxY = std::max(startPos.y, currentPos.y);
    
    sf::FloatRect selectionRect(minX, minY, maxX - minX, maxY - minY);
    
    if (selectionRect.width > 1 && selectionRect.height > 1) {
        Selection newSelection;
        newSelection.setRectangle(selectionRect, selectionMode);
        
        if (onSelectionChanged) {
            onSelectionChanged(newSelection);
        }
    }
}

void SelectTool::onMouseMoved(const sf::Vector2f& pos, Image* image) {
    if (!image || !isSelecting) return;
    
    currentPos = image->worldToImage(pos);
}
