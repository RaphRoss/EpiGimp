#include "Application.hpp"
#include "../tools/ToolFactory.hpp"

Application::Application(int width, int height, const std::string& title)
    : window(sf::VideoMode(width, height), title), canvas(width, height) {
    window.setFramerateLimit(60);
    currentTool = ToolFactory::createTool("pencil", canvas);
    
    setupMenus();
    setupToolPanel();
}

void Application::setupMenus() {
    menuBar.addMenu("File");
    
    menuBar.addMenuItem("File", "Enregistrer", [this]() {
        canvas.saveToFile("drawing.png");
    });
    
    menuBar.addMenuItem("File", "Importer", [this]() {
        canvas.loadFromFile("test_image.png");
    });
}

void Application::setupToolPanel() {
    toolPanel.addTool("pencil", "Pinceau", [this]() {
        currentTool = ToolFactory::createTool("pencil", canvas);
    });
    
    toolPanel.setSelectedTool("pencil");
}

void Application::handleCanvasInput(const sf::Event& event) {
    if (!currentTool) return;
    
    sf::Vector2i pixelPos;
    sf::Vector2f pos;
    
    if (event.type == sf::Event::MouseButtonPressed) {
        pixelPos = {event.mouseButton.x, event.mouseButton.y};
        pos = window.mapPixelToCoords(pixelPos);
        
        if (canvas.getBounds().contains(pos)) {
            sf::Vector2f canvasPos = pos - canvas.getPosition();
            currentTool->onMousePressed(canvasPos);
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased) {
        pixelPos = {event.mouseButton.x, event.mouseButton.y};
        pos = window.mapPixelToCoords(pixelPos);
        sf::Vector2f canvasPos = pos - canvas.getPosition();
        currentTool->onMouseReleased(canvasPos);
    }
    else if (event.type == sf::Event::MouseMoved) {
        pixelPos = {event.mouseMove.x, event.mouseMove.y};
        pos = window.mapPixelToCoords(pixelPos);
        sf::Vector2f canvasPos = pos - canvas.getPosition();
        currentTool->onMouseMoved(canvasPos);
    }
}

void Application::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        sf::Vector2i pixelPos;
        sf::Vector2f pos;

        if (event.type == sf::Event::MouseButtonPressed) {
            pixelPos = {event.mouseButton.x, event.mouseButton.y};
            pos = window.mapPixelToCoords(pixelPos);
            
            bool menuHandled = menuBar.handleClick(pos);
            
            if (!menuHandled && pos.y > 30) {
                toolPanel.handleClick(pos);
            }
        }
        
        handleCanvasInput(event);
    }
}

void Application::render() {
    window.clear(sf::Color(240, 240, 240));
    
    canvas.draw(window);
    toolPanel.draw(window);
    
    menuBar.draw(window);
    
    window.display();
}

void Application::run() {
    while (window.isOpen()) {
        processEvents();
        render();
    }
}
