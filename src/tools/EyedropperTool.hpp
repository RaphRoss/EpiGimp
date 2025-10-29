#pragma once
#include "Tool.hpp"
#include "../core/Image.hpp"

class EyedropperTool : public Tool {
public:
    void onMousePressed(const sf::Vector2f& pos, Image* image) override;
    void onMouseReleased(const sf::Vector2f& pos, Image* image) override;
    void onMouseMoved(const sf::Vector2f& pos, Image* image) override;
};
