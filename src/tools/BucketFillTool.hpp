#pragma once
#include "Tool.hpp"
#include "../core/Image.hpp"

class BucketFillTool : public Tool {
public:
    void onMousePressed(const sf::Vector2f& pos, Image* image) override;
    void onMouseReleased(const sf::Vector2f&, Image*) override {}
    void onMouseMoved(const sf::Vector2f&, Image*) override {}
    void setTolerance(float t) { tolerance = std::max(0.f, std::min(255.f, t)); }
private:
    float tolerance = 0.f;
};
