#include "Application.hpp"
#include <iostream>

Application::Application(int width, int height, const std::string& title)
    : window(sf::VideoMode(width, height), title), canvas(width, height) {
    window.setFramerateLimit(60);
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
    }
}

void Application::render() {
    window.clear(sf::Color::White);
    canvas.draw(window);
    window.display();
}
