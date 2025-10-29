#include "GradientTool.hpp"
#include "../core/ColorManager.hpp"
#include "../core/Command.hpp"
#include <cmath>

void GradientTool::onMousePressed(const sf::Vector2f& pos, Image* image) {
    if (!image) return;
    dragging = true;
    startPos = image->worldToImage(pos);
    endPos = startPos;
}

void GradientTool::onMouseMoved(const sf::Vector2f& pos, Image* image) {
    if (!dragging || !image) return;
    endPos = image->worldToImage(pos);
}

void GradientTool::onMouseReleased(const sf::Vector2f& pos, Image* image) {
    if (!image || !dragging) return;
    dragging = false;
    endPos = image->worldToImage(pos);
    sf::Image before = image->getImageData();
    sf::Image after = before;
    const Selection& sel = image->getSelection();
    sf::Color fg = ColorManager::instance().getForeground();
    sf::Color bg = ColorManager::instance().getBackground();
    sf::Vector2f a = startPos;
    sf::Vector2f b = endPos;
    float len2 = (b.x-a.x)*(b.x-a.x) + (b.y-a.y)*(b.y-a.y);
    float radius = std::sqrt(len2);
    for (unsigned int y=0; y<after.getSize().y; ++y) {
        for (unsigned int x=0; x<after.getSize().x; ++x) {
            sf::Vector2f p(static_cast<float>(x), static_cast<float>(y));
            if (!sel.isEmpty() && !sel.contains(p)) continue;
            float t=0.f;
            if (type == Linear) {
                if (len2 > 0) {
                    float proj = ((p.x-a.x)*(b.x-a.x) + (p.y-a.y)*(b.y-a.y)) / len2;
                    t = std::max(0.f, std::min(1.f, proj));
                } else t = 0.f;
            } else {
                float d = std::sqrt((p.x-a.x)*(p.x-a.x) + (p.y-a.y)*(p.y-a.y));
                t = radius > 0 ? std::max(0.f, std::min(1.f, d / radius)) : 0.f;
            }
            sf::Color from = fg;
            sf::Color to = mode == FGtoBG ? bg : sf::Color(fg.r, fg.g, fg.b, 0);
            float inv = 1.f - t;
            sf::Uint8 r = static_cast<sf::Uint8>(from.r * inv + to.r * t);
            sf::Uint8 g = static_cast<sf::Uint8>(from.g * inv + to.g * t);
            sf::Uint8 bch = static_cast<sf::Uint8>(from.b * inv + to.b * t);
            sf::Uint8 aout = static_cast<sf::Uint8>(from.a * inv + to.a * t);
            sf::Color dst = after.getPixel(x,y);
            float al = (opacity * aout) / 255.f;
            dst.r = static_cast<sf::Uint8>(dst.r * (1.f - al) + r * al);
            dst.g = static_cast<sf::Uint8>(dst.g * (1.f - al) + g * al);
            dst.b = static_cast<sf::Uint8>(dst.b * (1.f - al) + bch * al);
            dst.a = static_cast<sf::Uint8>(std::min(255.f, dst.a + aout * opacity));
            after.setPixel(x,y,dst);
        }
    }
    auto cmd = std::make_unique<ApplyImageCommand>(image, before, after);
    image->getHistoryManager().executeCommand(std::move(cmd));
}
