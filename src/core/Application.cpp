#include "Application.hpp"
#include "../tools/ToolFactory.hpp"
#include <cstdio>
#include <string>

Application::Application(int width, int height, const std::string& title)
    : window(sf::VideoMode(width, height), title), canvas(width, height) {
    window.setFramerateLimit(60);
    currentTool = ToolFactory::createTool("pencil", canvas);
    
    setupMenus();
    setupToolPanel();
}

void Application::setupMenus() {
    menuBar.addMenu("File");
    
    menuBar.addMenuItem("File", "Save", [this]() {
        saveFile();
    });
    
    menuBar.addMenuItem("File", "Save As", [this]() {
        saveAsFile();
    });
    
    menuBar.addMenuItem("File", "Open", [this]() {
        openFile();
    });
}

void Application::setupToolPanel() {
    toolPanel.addTool("pencil", "Pencil", [this]() {
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

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.control && event.key.code == sf::Keyboard::S) {
                saveFile();
            }
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

void Application::saveFile() {
    if (currentFilePath.empty()) {
        currentFilePath = saveFileDialog();
        if (currentFilePath.empty()) return;
    }
    canvas.saveToFile(currentFilePath);
}

void Application::saveAsFile() {
    std::string filePath = saveFileDialog();
    if (!filePath.empty()) {
        canvas.saveToFile(filePath);
        currentFilePath = filePath;
    }
}

void Application::openFile() {
    std::string filePath = openFileDialog();
    if (!filePath.empty()) {
        canvas.loadFromFile(filePath);
        currentFilePath = filePath;
    }
}

std::string Application::openFileDialog() {
    std::string command = "zenity --file-selection --file-filter='PNG files | *.png' --title='Open Image' 2>/dev/null";
    
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cout << "Enter file path to open: ";
        std::string path;
        std::getline(std::cin, path);
        return path;
    }
    
    char buffer[1024];
    std::string result = "";
    if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result = buffer;
        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }
    }
    pclose(pipe);
    
    if (result.empty()) {
        std::cout << "Enter file path to open: ";
        std::getline(std::cin, result);
    }
    
    return result;
}

std::string Application::saveFileDialog() {
    std::string command = "zenity --file-selection --save --file-filter='PNG files | *.png' --title='Save Image' --filename='untitled.png' 2>/dev/null";
    
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cout << "Enter file path to save: ";
        std::string path;
        std::getline(std::cin, path);
        if (path.find(".png") == std::string::npos) {
            path += ".png";
        }
        return path;
    }
    
    char buffer[1024];
    std::string result = "";
    if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result = buffer;
        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }
        
        if (!result.empty() && result.find(".png") == std::string::npos) {
            result += ".png";
        }
    }
    pclose(pipe);
    
    if (result.empty()) {
        std::cout << "Enter file path to save: ";
        std::getline(std::cin, result);
        if (result.find(".png") == std::string::npos) {
            result += ".png";
        }
    }
    
    return result;
}
