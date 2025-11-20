#pragma once
#include "Tool.hpp"
#include "../core/Image.hpp"

class GradientTool : public Tool {
public:
    enum Type { Linear, Radial };
    enum Mode { FGtoBG, FGtoTransparent };
    void onMousePressed(const sf::Vector2f& pos, Image* image, sf::Mouse::Button button = sf::Mouse::Left) override;
    void onMouseReleased(const sf::Vector2f& pos, Image* image, sf::Mouse::Button button = sf::Mouse::Left) override;
    void onMouseMoved(const sf::Vector2f& pos, Image* image) override;
    void setType(Type t) { type = t; }
    void setMode(Mode m) { mode = m; }
    void setOpacity(float o) { opacity = std::max(0.f, std::min(1.f, o)); }
private:
    bool dragging=false;
    sf::Vector2f startPos;
    sf::Vector2f endPos;
    Type type = Linear;
    Mode mode = FGtoBG;
    float opacity = 1.f;
};
