#include "PencilTool.hpp"
#include <cmath>
#include <algorithm>
#include "../core/ColorManager.hpp"

PencilTool::PencilTool() {}

void PencilTool::onMousePressed(const sf::Vector2f& pos, Image* image, sf::Mouse::Button button) {
    if (!image) return;
    drawing = true;
    lastPos = image->worldToImage(pos);
    currentStroke = std::make_unique<StrokeCommand>(image);
    
    // Use foreground color for left click, background color for right click
    brushColor = (button == sf::Mouse::Right) ? 
                  ColorManager::instance().getBackground() : 
                  ColorManager::instance().getForeground();
    
    auto& texture = image->getTexture();
    drawBrushStroke(*lastPos, texture);
    texture.display();
    
    currentStroke->addPoint(*lastPos, brushColor);
}

void PencilTool::onMouseReleased(const sf::Vector2f&, Image* image, sf::Mouse::Button) {
    if (!image || !drawing || !currentStroke) return;
    
    drawing = false;
    image->getHistoryManager().executeCommand(std::move(currentStroke));
    image->markAsModified();
    lastPos.reset();
}

void PencilTool::onMouseMoved(const sf::Vector2f& pos, Image* image) {
    if (!drawing || !lastPos.has_value() || !image || !currentStroke) return;
    
    sf::Vector2f imagePos = image->worldToImage(pos);
    // Keep the color that was set during onMousePressed
    
    auto& texture = image->getTexture();
    drawSmoothLine(*lastPos, imagePos, texture);
    texture.display();
    
    currentStroke->addPoint(imagePos, brushColor);
    lastPos = imagePos;
}

void PencilTool::ensureStamp() {
    if (!stampDirty) return;
    unsigned int sz = static_cast<unsigned int>(std::max(1.f, brushSize));
    sf::Image img; img.create(sz, sz, sf::Color::Transparent);
    float r = sz / 2.f;
    for (unsigned int y=0; y<sz; ++y) {
        for (unsigned int x=0; x<sz; ++x) {
            float dx = (x + 0.5f) - r;
            float dy = (y + 0.5f) - r;
            float dist = std::sqrt(dx*dx + dy*dy);
            if (dist > r) continue;
            float t = dist / r;
            float edge = std::pow(std::max(0.f, 1.f - t), brushHardness);
            sf::Uint8 a = static_cast<sf::Uint8>(255 * brushOpacity * edge);
            img.setPixel(x,y, sf::Color(brushColor.r, brushColor.g, brushColor.b, a));
        }
    }
    brushStamp.loadFromImage(img);
    stampDirty = false;
}

void PencilTool::drawBrushStroke(const sf::Vector2f& position, sf::RenderTexture& texture) {
    ensureStamp();
    sf::Sprite sp(brushStamp);
    sp.setPosition(position.x - brushStamp.getSize().x/2.f, position.y - brushStamp.getSize().y/2.f);
    sf::RenderStates states; states.blendMode = sf::BlendAlpha;
    texture.draw(sp, states);
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
