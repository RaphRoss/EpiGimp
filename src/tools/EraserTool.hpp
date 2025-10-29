#pragma once
#include "Tool.hpp"
#include "../core/Image.hpp"
#include "../core/Command.hpp"

class EraserTool : public Tool {
public:
    enum Mode { ToBackground, ToTransparent };
    EraserTool();
    void onMousePressed(const sf::Vector2f& pos, Image* image) override;
    void onMouseReleased(const sf::Vector2f& pos, Image* image) override;
    void onMouseMoved(const sf::Vector2f& pos, Image* image) override;
    void setSize(float s) { size = s; dirty = true; }
    void setOpacity(float o) { opacity = std::max(0.f, std::min(1.f, o)); dirty = true; }
    void setHardness(float h) { hardness = std::max(0.f, std::min(1.f, h)); dirty = true; }
    void setMode(Mode m) { mode = m; }

private:
    void ensureStamp();
    void stampAt(Image* image, const sf::Vector2f& ipos);
    sf::Texture stamp;
    bool dirty = true;
    float size = 20.f;
    float opacity = 1.f;
    float hardness = 1.f;
    bool erasing = false;
    sf::Vector2f lastPos;
    Mode mode = ToBackground;
    std::unique_ptr<ApplyImageCommand> pending;
};
