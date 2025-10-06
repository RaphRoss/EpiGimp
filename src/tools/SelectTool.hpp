#pragma once
#include "Tool.hpp"
#include "../core/Selection.hpp"
#include <functional>

class SelectTool : public Tool {
public:
    SelectTool();
    
    void onMousePressed(const sf::Vector2f& pos, Image* image) override;
    void onMouseReleased(const sf::Vector2f& pos, Image* image) override;
    void onMouseMoved(const sf::Vector2f& pos, Image* image) override;
    
    void setSelectionMode(SelectionMode mode) { selectionMode = mode; }
    SelectionMode getSelectionMode() const { return selectionMode; }
    
    void setOnSelectionChanged(std::function<void(const Selection&)> callback) { 
        onSelectionChanged = callback; 
    }

private:
    bool isSelecting = false;
    sf::Vector2f startPos;
    sf::Vector2f currentPos;
    SelectionMode selectionMode = SelectionMode::Replace;
    
    std::function<void(const Selection&)> onSelectionChanged;
};
