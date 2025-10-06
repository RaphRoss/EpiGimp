#include "PencilTool.hpp"

PencilTool::PencilTool() {}

void PencilTool::onMousePressed(const sf::Vector2f& pos, Image* image) {
    if (!image) return;
    drawing = true;
    lastPos = image->worldToImage(pos);
}

void PencilTool::onMouseReleased(const sf::Vector2f& /* pos */, Image* /* image */) {
    drawing = false;
    lastPos.reset();
}

void PencilTool::onMouseMoved(const sf::Vector2f& pos, Image* image) {
    if (!drawing || !lastPos.has_value() || !image) return;
    sf::Vector2f imagePos = image->worldToImage(pos);
    sf::Vertex line[] = {
        sf::Vertex(*lastPos, sf::Color::Black),
        sf::Vertex(imagePos, sf::Color::Black)
    };
    auto& texture = image->getTexture();
    texture.draw(line, 2, sf::Lines);
    texture.display();
    image->markAsModified();
    lastPos = imagePos;
}
