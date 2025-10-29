#include "EraserTool.hpp"
#include "../core/ColorManager.hpp"
#include <cmath>

EraserTool::EraserTool() {}

void EraserTool::ensureStamp() {
    if (!dirty) return;
    unsigned int sz = static_cast<unsigned int>(std::max(1.f, size));
    sf::Image img; img.create(sz, sz, sf::Color::Transparent);
    float r = sz / 2.f;
    for (unsigned int y=0; y<sz; ++y) {
        for (unsigned int x=0; x<sz; ++x) {
            float dx = (x + 0.5f) - r;
            float dy = (y + 0.5f) - r;
            float dist = std::sqrt(dx*dx + dy*dy);
            if (dist > r) continue;
            float t = dist / r;
            float edge = std::pow(std::max(0.f, 1.f - t), hardness);
            sf::Uint8 a = static_cast<sf::Uint8>(255 * opacity * edge);
            sf::Color base = ColorManager::instance().getBackground();
            if (mode == ToBackground) img.setPixel(x,y, sf::Color(base.r, base.g, base.b, a));
            else img.setPixel(x,y, sf::Color(255,255,255, 255 - a));
        }
    }
    stamp.loadFromImage(img);
    dirty = false;
}

void EraserTool::stampAt(Image* image, const sf::Vector2f& ipos) {
    if (mode == ToBackground) {
        ensureStamp();
        sf::Sprite sp(stamp);
        sp.setPosition(ipos.x - stamp.getSize().x/2.f, ipos.y - stamp.getSize().y/2.f);
        auto& rt = image->getTexture();
        sf::RenderStates st; st.blendMode = sf::BlendAlpha;
        rt.draw(sp, st);
        rt.display();
    } else {
        sf::Image before = image->getImageData();
        sf::Image after = before;
        unsigned int sz = static_cast<unsigned int>(std::max(1.f, size));
        float r = sz / 2.f;
        int cx = static_cast<int>(std::floor(ipos.x));
        int cy = static_cast<int>(std::floor(ipos.y));
        for (int y = cy - static_cast<int>(r); y <= cy + static_cast<int>(r); ++y) {
            for (int x = cx - static_cast<int>(r); x <= cx + static_cast<int>(r); ++x) {
                if (x < 0 || y < 0 || x >= static_cast<int>(after.getSize().x) || y >= static_cast<int>(after.getSize().y)) continue;
                float dx = x + 0.5f - ipos.x;
                float dy = y + 0.5f - ipos.y;
                float dist = std::sqrt(dx*dx + dy*dy);
                if (dist > r) continue;
                float t = dist / r;
                float edge = std::pow(std::max(0.f, 1.f - t), hardness);
                float a = opacity * edge;
                sf::Color c = after.getPixel(x,y);
                float newA = c.a * (1.f - a);
                c.a = static_cast<sf::Uint8>(std::max(0.f, std::min(255.f, newA)));
                after.setPixel(x,y,c);
            }
        }
        auto cmd = std::make_unique<ApplyImageCommand>(image, before, after);
        image->getHistoryManager().executeCommand(std::move(cmd));
    }
}

void EraserTool::onMousePressed(const sf::Vector2f& pos, Image* image) {
    if (!image) return;
    erasing = true;
    lastPos = image->worldToImage(pos);
    stampAt(image, lastPos);
}

void EraserTool::onMouseReleased(const sf::Vector2f&, Image* image) {
    if (!image) return;
    erasing = false;
    image->markAsModified();
}

void EraserTool::onMouseMoved(const sf::Vector2f& pos, Image* image) {
    if (!erasing || !image) return;
    sf::Vector2f ip = image->worldToImage(pos);
    sf::Vector2f start = lastPos;
    sf::Vector2f end = ip;
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    float dist = std::sqrt(dx*dx + dy*dy);
    int steps = static_cast<int>(dist);
    for (int i=0;i<=steps;i++) {
        float t = steps ? static_cast<float>(i)/steps : 1.f;
        sf::Vector2f p(start.x + t*dx, start.y + t*dy);
        stampAt(image, p);
    }
    lastPos = ip;
}
