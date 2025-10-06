#include "PencilTool.hpp"

PencilTool::PencilTool() {}

void PencilTool::onMousePressed(const sf::Vector2f& pos, Image* image) {
    if (!image) return;
    drawing = true;
    lastPos = image->worldToImage(pos);
    currentStroke = std::make_unique<StrokeCommand>(image);
    currentStroke->addPoint(*lastPos, sf::Color::Black);
}

void PencilTool::onMouseReleased(const sf::Vector2f&, Image* image) {
    if (!image || !drawing || !currentStroke) return;
    
    drawing = false;
    image->getHistoryManager().executeCommand(std::move(currentStroke));
    image->markAsModified();
    lastPos.reset();
}

void PencilTool::onMouseMoved(const sf::Vector2f& pos, Image* image) {
    if (!drawing || !lastPos.has_value() || !image || !currentStroke) return;
    
    sf::Vector2f imagePos = image->worldToImage(pos);
    
    sf::Vertex line[] = {
        sf::Vertex(*lastPos, sf::Color::Black),
        sf::Vertex(imagePos, sf::Color::Black)
    };
    auto& texture = image->getTexture();
    texture.draw(line, 2, sf::Lines);
    texture.display();
    
    currentStroke->addPoint(imagePos, sf::Color::Black);
    
    lastPos = imagePos;
}
