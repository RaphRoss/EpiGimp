#include "Application.hpp"
#include "../tools/ToolFactory.hpp"

Application::Application(int width, int height, const std::string& title)
    : window(sf::VideoMode(width, height), title), canvas(width, height) {
    window.setFramerateLimit(60);

    currentTool = ToolFactory::createTool("pencil", canvas);
}

void Application::run() {
    while (window.isOpen()) {
        processEvents();
        render();
    }
}

void Application::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (!currentTool) continue;

        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
            sf::Vector2f pos = window.mapPixelToCoords(pixelPos);
            currentTool->onMousePressed(pos);
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
            sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
            sf::Vector2f pos = window.mapPixelToCoords(pixelPos);
            currentTool->onMouseReleased(pos);
        }
        else if (event.type == sf::Event::MouseMoved) {
            sf::Vector2i pixelPos(event.mouseMove.x, event.mouseMove.y);
            sf::Vector2f pos = window.mapPixelToCoords(pixelPos);
            currentTool->onMouseMoved(pos);
        }
    }
}

void Application::render() {
    window.clear(sf::Color::White);
    canvas.draw(window);
    window.display();
}
