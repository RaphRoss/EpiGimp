#include "ToolPanel.hpp"
#include <iostream>

sf::Font ToolPanel::font;
bool ToolPanel::fontLoaded = false;
const float ToolPanel::PANEL_WIDTH = 200.0f;
const float ToolPanel::BUTTON_HEIGHT = 40.0f;

ToolPanel::ToolPanel() {
    if (!fontLoaded) {
        if (!font.loadFromFile("src/assets/fonts/DejaVuSans-Bold.ttf")) {
            std::cerr << "Erreur: Impossible de charger la police pour le panneau d'outils !" << std::endl;
            fontLoaded = false;
        } else {
            fontLoaded = true;
        }
    }
    background.setPosition(0, 30);
    background.setSize({PANEL_WIDTH, 570});
    background.setFillColor(sf::Color(37, 37, 38));
    background.setOutlineColor(sf::Color(63, 63, 70));
    background.setOutlineThickness(1);
}

void ToolPanel::addTool(const std::string& name, const std::string& displayName, std::function<void()> callback) {
    ToolButton button;
    button.toolName = name;
    button.callback = callback;
    
    button.shape.setSize({PANEL_WIDTH - 20, BUTTON_HEIGHT - 5});
    button.shape.setFillColor(sf::Color(51, 51, 55));
    button.shape.setOutlineColor(sf::Color(63, 63, 70));
    button.shape.setOutlineThickness(1);
    if (fontLoaded) {
        button.label.setFont(font);
    }
    button.label.setString(displayName);
    button.label.setCharacterSize(14);
    button.label.setFillColor(sf::Color(204, 204, 204));
    
    toolButtons.push_back(button);
    updateLayout();
}

void ToolPanel::updateLayout() {
    for (size_t i = 0; i < toolButtons.size(); ++i) {
        float yPos = 40 + i * (BUTTON_HEIGHT + 5);
        toolButtons[i].shape.setPosition(10, yPos);
        sf::FloatRect textBounds = toolButtons[i].label.getLocalBounds();
        toolButtons[i].label.setPosition(15, yPos + (BUTTON_HEIGHT - textBounds.height) / 2 - textBounds.top);
        if (toolButtons[i].selected) {
            toolButtons[i].shape.setFillColor(sf::Color(0, 122, 204));
        } else {
            toolButtons[i].shape.setFillColor(sf::Color(51, 51, 55));
        }
    }
}

void ToolPanel::draw(sf::RenderWindow& window) {
    window.draw(background);
    for (auto& button : toolButtons) {
        window.draw(button.shape);
        if (fontLoaded) {
            window.draw(button.label);
        }
    }
}

void ToolPanel::handleClick(const sf::Vector2f& mousePos) {
    for (auto& button : toolButtons) {
        if (button.shape.getGlobalBounds().contains(mousePos)) {
            if (button.callback) {
                button.callback();
            }
            setSelectedTool(button.toolName);
            break;
        }
    }
}

void ToolPanel::setSelectedTool(const std::string& toolName) {
    for (auto& button : toolButtons) {
        button.selected = (button.toolName == toolName);
    }
    updateLayout();
}
