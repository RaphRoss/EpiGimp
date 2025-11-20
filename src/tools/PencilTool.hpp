#pragma once
#include "Tool.hpp"
#include "../core/Image.hpp"
#include "../core/Command.hpp"
#include <optional>
#include <memory>

class PencilTool : public Tool {
public:
    PencilTool();

    void onMousePressed(const sf::Vector2f& pos, Image* image, sf::Mouse::Button button = sf::Mouse::Left) override;
    void onMouseReleased(const sf::Vector2f& pos, Image* image, sf::Mouse::Button button = sf::Mouse::Left) override;
    void onMouseMoved(const sf::Vector2f& pos, Image* image) override;

    void setBrushSize(float size) { brushSize = size; }
    void setBrushColor(const sf::Color& color) { brushColor = color; }
    void setBrushOpacity(float opacity) { brushOpacity = opacity; }
    void setBrushHardness(float h) { brushHardness = h; stampDirty = true; }

private:
    void drawBrushStroke(const sf::Vector2f& position, sf::RenderTexture& texture);
    void drawSmoothLine(const sf::Vector2f& start, const sf::Vector2f& end, sf::RenderTexture& texture);
    sf::Color blendColors(const sf::Color& base, const sf::Color& overlay, float alpha);
    void ensureStamp();
    sf::Texture brushStamp;
    bool stampDirty = true;
    
    bool drawing = false;
    std::optional<sf::Vector2f> lastPos;
    std::unique_ptr<StrokeCommand> currentStroke;
    
    float brushSize = 5.0f;
    sf::Color brushColor = sf::Color::Black;
    float brushOpacity = 1.0f;
    float brushHardness = 1.0f;
};
