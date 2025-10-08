#include "PencilTool.hpp"
#include <cmath>
#include <algorithm>

PencilTool::PencilTool() {}

void PencilTool::onMousePressed(const sf::Vector2f& pos, Image* image) {
    if (!image) return;
    drawing = true;
    lastPos = image->worldToImage(pos);
    currentStroke = std::make_unique<StrokeCommand>(image);
    
    auto& texture = image->getTexture();
    drawBrushStroke(*lastPos, texture);
    texture.display();
    
    currentStroke->addPoint(*lastPos, brushColor);
}

void PencilTool::onMouseReleased(const sf::Vector2f&, Image* image) {
    if (!image || !drawing || !currentStroke) return;
    
    drawing = false;
    image->getHistoryManager().executeCommand(std::move(currentStroke));
    image->markAsModified();
    lastPos.reset();
}

void PencilTool::onMouseMoved(const sf::Vector2f& pos, Image* image) {
    if (!drawing || !lastPos.has_value() || !image || !currentStroke) return;
    
    sf::Vector2f imagePos = image->worldToImage(pos);
    
    auto& texture = image->getTexture();
    drawSmoothLine(*lastPos, imagePos, texture);
    texture.display();
    
    currentStroke->addPoint(imagePos, brushColor);
    lastPos = imagePos;
}

void PencilTool::drawBrushStroke(const sf::Vector2f& position, sf::RenderTexture& texture) {
    sf::CircleShape brush(brushSize / 2.0f);
    brush.setFillColor(sf::Color(brushColor.r, brushColor.g, brushColor.b, 
                                static_cast<sf::Uint8>(255 * brushOpacity)));
    brush.setPosition(position.x - brushSize / 2.0f, position.y - brushSize / 2.0f);
    
    sf::RenderStates states;
    states.blendMode = sf::BlendAlpha;
    texture.draw(brush, states);
}

void PencilTool::drawSmoothLine(const sf::Vector2f& start, const sf::Vector2f& end, sf::RenderTexture& texture) {
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    
    if (distance < 1.0f) {
        drawBrushStroke(end, texture);
        return;
    }
    
    int steps = static_cast<int>(distance);
    for (int i = 0; i <= steps; ++i) {
        float t = static_cast<float>(i) / steps;
        sf::Vector2f point(start.x + t * dx, start.y + t * dy);
        drawBrushStroke(point, texture);
    }
}

sf::Color PencilTool::blendColors(const sf::Color& base, const sf::Color& overlay, float alpha) {
    alpha = std::clamp(alpha, 0.0f, 1.0f);
    
    return sf::Color(
        static_cast<sf::Uint8>(base.r * (1.0f - alpha) + overlay.r * alpha),
        static_cast<sf::Uint8>(base.g * (1.0f - alpha) + overlay.g * alpha),
        static_cast<sf::Uint8>(base.b * (1.0f - alpha) + overlay.b * alpha),
        255
    );
}
