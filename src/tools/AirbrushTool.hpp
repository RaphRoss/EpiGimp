#pragma once
#include "Tool.hpp"
#include "../core/Image.hpp"
#include <random>

class AirbrushTool : public Tool {
public:
    AirbrushTool();
    void onMousePressed(const sf::Vector2f& pos, Image* image, sf::Mouse::Button button = sf::Mouse::Left) override;
    void onMouseReleased(const sf::Vector2f& pos, Image* image, sf::Mouse::Button button = sf::Mouse::Left) override;
    void onMouseMoved(const sf::Vector2f& pos, Image* image) override;
    void setSize(float s) { size=s; }
    void setOpacity(float o) { opacity = std::max(0.f, std::min(1.f, o)); }
    void setFlow(float f) { flow = std::max(0.f, std::min(1.f, f)); }

private:
    void spray(Image* image, const sf::Vector2f& ipos, int count);
    bool active=false;
    float size=30.f;
    float opacity=0.2f;
    float flow=0.5f;
    std::mt19937 rng;
    std::uniform_real_distribution<float> dist;
};
