#pragma once
#include <SFML/Graphics.hpp>
#include "Canvas.hpp"
#include "../tools/Tool.hpp"
#include <memory>

class Application {
public:
    Application(int width, int height, const std::string& title);
    void run();

private:
    void processEvents();
    void render();

private:
    sf::RenderWindow window;
    Canvas canvas;

    std::unique_ptr<Tool> currentTool;
};
