#include "AirbrushTool.hpp"
#include "../core/ColorManager.hpp"
#include <cmath>

AirbrushTool::AirbrushTool() : rng(std::random_device{}()), dist(0.f,1.f) {}

void AirbrushTool::spray(Image* image, const sf::Vector2f& ipos, int count) {
    auto& rt = image->getTexture();
    sf::RenderStates st; st.blendMode = sf::BlendAlpha;
    sf::Color base = ColorManager::instance().getForeground();
    for (int i=0;i<count;i++) {
        float r = size/2.f * std::sqrt(dist(rng));
        float a = 2*M_PI*dist(rng);
        sf::Vector2f p(ipos.x + r*std::cos(a), ipos.y + r*std::sin(a));
        sf::CircleShape dot(1.5f);
        dot.setFillColor(sf::Color(base.r, base.g, base.b, static_cast<sf::Uint8>(opacity*255)));
        dot.setPosition(p.x - 1.5f, p.y - 1.5f);
        rt.draw(dot, st);
    }
    rt.display();
}

void AirbrushTool::onMousePressed(const sf::Vector2f& pos, Image* image, sf::Mouse::Button button) {
    (void)button;
    if (!image) return;
    active = true;
    sf::Vector2f ip = image->worldToImage(pos);
    spray(image, ip, static_cast<int>(50 * flow));
}

void AirbrushTool::onMouseReleased(const sf::Vector2f&, Image* image, sf::Mouse::Button button) {
    (void)button;
    if (!image) return;
    active = false;
    image->markAsModified();
}

void AirbrushTool::onMouseMoved(const sf::Vector2f& pos, Image* image) {
    if (!active || !image) return;
    sf::Vector2f ip = image->worldToImage(pos);
    spray(image, ip, static_cast<int>(80 * flow));
}
