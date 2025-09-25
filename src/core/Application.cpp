#include "Application.hpp"
#include "../tools/ToolFactory.hpp"
#include <cstdio>
#include <string>

Application::Application(int width, int height, const std::string& title)
    : window(sf::VideoMode(width, height), title) {
    window.setFramerateLimit(60);
    currentTool = ToolFactory::createTool("pencil");
    imageManager.setViewportSize({static_cast<float>(width - 220), static_cast<float>(height - 50)});
    setupMenus();
    setupToolPanel();
    newImageDialog.setOnConfirm([this](int width, int height, const std::string& name) {
        imageManager.createNewImage(width, height, name);
    });
}

void Application::setupMenus() {
    menuBar.addMenu("File");
    
    menuBar.addMenuItem("File", "New Image", [this]() {
        newImage();
    });
    
    menuBar.addMenuItem("File", "Open", [this]() {
        openFile();
    });
    
    menuBar.addMenuItem("File", "Save", [this]() {
        saveFile();
    });
    
    menuBar.addMenuItem("File", "Save As", [this]() {
        saveAsFile();
    });
}

void Application::setupToolPanel() {
    toolPanel.addTool("pencil", "Pencil", [this]() {
        currentTool = ToolFactory::createTool("pencil");
    });
    
    toolPanel.setSelectedTool("pencil");
}

void Application::handleImageInput(const sf::Event& event) {
    if (!currentTool) return;
    Image* currentImage = imageManager.getCurrentImage();
    if (!currentImage) return;
    sf::Vector2i pixelPos;
    sf::Vector2f pos;
    if (event.type == sf::Event::MouseButtonPressed) {
        pixelPos = {event.mouseButton.x, event.mouseButton.y};
        pos = window.mapPixelToCoords(pixelPos);
        if (imageManager.isPositionInCurrentImage(pos)) {
            currentTool->onMousePressed(pos, currentImage);
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased) {
        pixelPos = {event.mouseButton.x, event.mouseButton.y};
        pos = window.mapPixelToCoords(pixelPos);
        currentTool->onMouseReleased(pos, currentImage);
    }
    else if (event.type == sf::Event::MouseMoved) {
        pixelPos = {event.mouseMove.x, event.mouseMove.y};
        pos = window.mapPixelToCoords(pixelPos);
        currentTool->onMouseMoved(pos, currentImage);
    }
}

void Application::handleZoom(const sf::Event& event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f pos = window.mapPixelToCoords(pixelPos);
        if (imageManager.isPositionInCurrentImage(pos)) {
            if (event.mouseWheelScroll.delta > 0) {
                imageManager.zoomIn();
            } else {
                imageManager.zoomOut();
            }
        }
    }
}

void Application::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        newImageDialog.handleEvent(event);
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.control && event.key.code == sf::Keyboard::S) {
                saveFile();
            } else if (event.key.control && event.key.code == sf::Keyboard::N) {
                newImage();
            } else if (event.key.control && event.key.code == sf::Keyboard::O) {
                openFile();
            } else if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal) {
                imageManager.zoomIn();
            } else if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Hyphen) {
                imageManager.zoomOut();
            } else if (event.key.code == sf::Keyboard::Num0) {
                imageManager.resetZoom();
            } else if (event.key.control && event.key.code == sf::Keyboard::Tab) {
                imageManager.nextImage();
            }
        }

        sf::Vector2i pixelPos;
        sf::Vector2f pos;

        if (event.type == sf::Event::MouseButtonPressed) {
            pixelPos = {event.mouseButton.x, event.mouseButton.y};
            pos = window.mapPixelToCoords(pixelPos);
            
            // Ne gérer les clics sur l'interface que si le dialogue n'est pas visible
            if (!newImageDialog.isVisible()) {
                bool menuHandled = menuBar.handleClick(pos);
                
                if (!menuHandled && pos.y > 30) {
                    toolPanel.handleClick(pos);
                }
            }
        }
        
        if (event.type == sf::Event::MouseMoved) {
            pixelPos = {event.mouseMove.x, event.mouseMove.y};
            pos = window.mapPixelToCoords(pixelPos);
            
            // Gérer le survol des menus
            if (!newImageDialog.isVisible()) {
                menuBar.handleMouseMove(pos);
            }
        }
        
        // Gérer les entrées sur l'image
        if (!newImageDialog.isVisible()) {
            handleImageInput(event);
            handleZoom(event);
        }
    }
}

void Application::render() {
    window.clear(sf::Color(45, 45, 48));
    imageManager.draw(window);
    toolPanel.draw(window);
    menuBar.draw(window);
    newImageDialog.draw(window);
    
    window.display();
}

void Application::run() {
    while (window.isOpen()) {
        processEvents();
        render();
    }
}

void Application::saveFile() {
    Image* currentImage = imageManager.getCurrentImage();
    if (!currentImage) return;
    
    if (currentFilePath.empty()) {
        currentFilePath = saveFileDialog();
        if (currentFilePath.empty()) return;
    }
    currentImage->saveToFile(currentFilePath);
}

void Application::saveAsFile() {
    Image* currentImage = imageManager.getCurrentImage();
    if (!currentImage) return;
    
    std::string filePath = saveFileDialog();
    if (!filePath.empty()) {
        currentImage->saveToFile(filePath);
        currentFilePath = filePath;
    }
}

void Application::openFile() {
    std::string filePath = openFileDialog();
    if (!filePath.empty()) {
        imageManager.openImage(filePath);
        currentFilePath = filePath;
    }
}

void Application::newImage() {
    newImageDialog.show();
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
