#pragma once
#include "Tool.hpp"
#include "../core/Image.hpp"
#include "../core/Command.hpp"
#include <optional>
#include <memory>

class PencilTool : public Tool {
public:
    PencilTool();

    void onMousePressed(const sf::Vector2f& pos, Image* image) override;
    void onMouseReleased(const sf::Vector2f& pos, Image* image) override;
    void onMouseMoved(const sf::Vector2f& pos, Image* image) override;

private:
    bool drawing = false;
    std::optional<sf::Vector2f> lastPos;
    std::unique_ptr<StrokeCommand> currentStroke;
};
