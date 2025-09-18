#pragma once
#include "Tool.hpp"
#include "../core/Canvas.hpp"
#include <optional>

class PencilTool : public Tool {
public:
    PencilTool(Canvas& canvas);

    void onMousePressed(const sf::Vector2f& pos) override;
    void onMouseReleased(const sf::Vector2f& pos) override;
    void onMouseMoved(const sf::Vector2f& pos) override;

private:
    Canvas& canvas;
    bool drawing = false;
    std::optional<sf::Vector2f> lastPos; // Pour tracer des segments
};
