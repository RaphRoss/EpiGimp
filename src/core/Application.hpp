#pragma once
#include <SFML/Graphics.hpp>
#include "Canvas.hpp"
#include "../tools/Tool.hpp"
#include <memory>
#include "../ui/MenuBar.hpp"
#include "../ui/ToolPanel.hpp"
#include <iostream>

class Application {
public:
    Application(int width, int height, const std::string& title);
    void run();

private:
    void processEvents();
    void render();
    void setupMenus();
    void setupToolPanel();
    void handleCanvasInput(const sf::Event& event);
    void saveFile();
    void saveAsFile();
    void openFile();
    std::string openFileDialog();
    std::string saveFileDialog();

private:
    sf::RenderWindow window;
    Canvas canvas;
    std::unique_ptr<Tool> currentTool;
    MenuBar menuBar;
    ToolPanel toolPanel;
    std::string currentFilePath;
};
