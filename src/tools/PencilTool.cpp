#include "PencilTool.hpp"

PencilTool::PencilTool(Canvas& c) : canvas(c) {}

void PencilTool::onMousePressed(const sf::Vector2f& pos) {
    drawing = true;
    lastPos = pos;
}

void PencilTool::onMouseReleased(const sf::Vector2f& pos) {
    drawing = false;
    lastPos.reset();
}

void PencilTool::onMouseMoved(const sf::Vector2f& pos) {
    if (!drawing || !lastPos.has_value()) return;

    sf::Vertex line[] = {
        sf::Vertex(*lastPos, sf::Color::Black),
        sf::Vertex(pos, sf::Color::Black)
    };

    auto& texture = canvas.getTexture();
    texture.draw(line, 2, sf::Lines);
    texture.display();

    lastPos = pos;
}
