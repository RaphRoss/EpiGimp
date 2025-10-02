#include "StatusBar.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

const float StatusBar::STATUS_BAR_HEIGHT = 25.0f;
sf::Font StatusBar::font;
bool StatusBar::fontLoaded = false;

StatusBar::StatusBar(float windowWidth) {
    if (!fontLoaded) {
        if (!font.loadFromFile("src/assets/fonts/DejaVuSans-Bold.ttf")) {
            std::cerr << "Erreur: Impossible de charger la police pour la barre d'état !" << std::endl;
            fontLoaded = false;
        } else {
            fontLoaded = true;
        }
    }
    
    // Background
    background.setSize({windowWidth, STATUS_BAR_HEIGHT});
    background.setPosition(0, 1080 - STATUS_BAR_HEIGHT); // Bottom of window
    background.setFillColor(sf::Color(37, 37, 38));
    background.setOutlineColor(sf::Color(63, 63, 70));
    background.setOutlineThickness(1);
    
    // Initialize texts
    if (fontLoaded) {
        mouseText.setFont(font);
        zoomText.setFont(font);
        imageText.setFont(font);
        toolText.setFont(font);
    }
    
    mouseText.setCharacterSize(12);
    zoomText.setCharacterSize(12);
    imageText.setCharacterSize(12);
    toolText.setCharacterSize(12);
    
    mouseText.setFillColor(sf::Color(204, 204, 204));
    zoomText.setFillColor(sf::Color(204, 204, 204));
    imageText.setFillColor(sf::Color(204, 204, 204));
    toolText.setFillColor(sf::Color(204, 204, 204));
    
    mouseText.setString("Mouse: (0, 0)");
    zoomText.setString("Zoom: 100%");
    imageText.setString("No image");
    toolText.setString("Tool: Pencil");
    
    updateLayout();
}

void StatusBar::draw(sf::RenderWindow& window) {
    window.draw(background);
    
    if (fontLoaded) {
        window.draw(mouseText);
        window.draw(zoomText);
        window.draw(imageText);
        window.draw(toolText);
    }
}

void StatusBar::updateMousePosition(const sf::Vector2f& imagePos, float zoom) {
    std::stringstream ss;
    ss << "Mouse: (" << std::fixed << std::setprecision(0) 
       << imagePos.x / zoom << ", " << imagePos.y / zoom << ")";
    mouseText.setString(ss.str());
    updateLayout();
}

void StatusBar::updateZoomLevel(float zoom) {
    std::stringstream ss;
    ss << "Zoom: " << std::fixed << std::setprecision(0) << (zoom * 100) << "%";
    zoomText.setString(ss.str());
    updateLayout();
}

void StatusBar::updateImageInfo(const std::string& imageName, int width, int height) {
    std::stringstream ss;
    ss << imageName << " (" << width << "x" << height << ")";
    imageText.setString(ss.str());
    updateLayout();
}

void StatusBar::updateToolInfo(const std::string& toolName) {
    toolText.setString("Tool: " + toolName);
    updateLayout();
}

void StatusBar::updateLayout() {
    float y = background.getPosition().y + 5;
    
    mouseText.setPosition(10, y);
    zoomText.setPosition(150, y);
    imageText.setPosition(250, y);
    toolText.setPosition(500, y);
}
