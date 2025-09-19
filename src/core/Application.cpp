#include "Application.hpp"
#include "../tools/ToolFactory.hpp"

Application::Application(int width, int height, const std::string& title)
    : window(sf::VideoMode(width, height), title), canvas(width, height) {
    window.setFramerateLimit(60);
    currentTool = ToolFactory::createTool("pencil", canvas);
    Button saveBtn({10, 10}, {80, 30}, "Save");
    saveBtn.setCallback([this]() {
        canvas.saveToFile("drawing.png");
        std::cout << "Image sauvegardée : drawing.png" << std::endl;
    });
    toolbar.addButton(saveBtn);
}

void Application::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        sf::Vector2i pixelPos;
        sf::Vector2f pos;

        if (!currentTool) continue;

        if (event.type == sf::Event::MouseButtonPressed) {
            pixelPos = {event.mouseButton.x, event.mouseButton.y};
            pos = window.mapPixelToCoords(pixelPos);
            toolbar.handleClick(pos);
            if (pos.y > 50)
                currentTool->onMousePressed(pos);
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
            pixelPos = {event.mouseButton.x, event.mouseButton.y};
            pos = window.mapPixelToCoords(pixelPos);
            currentTool->onMouseReleased(pos);
        }
        else if (event.type == sf::Event::MouseMoved) {
            pixelPos = {event.mouseMove.x, event.mouseMove.y};
            pos = window.mapPixelToCoords(pixelPos);
            currentTool->onMouseMoved(pos);
        }
    }
}

void Application::render() {
    window.clear(sf::Color::White);
    canvas.draw(window);
    toolbar.draw(window);
    window.display();
}


void Application::run() {
    while (window.isOpen()) {
        processEvents();
        render();
    }
}
