#include "ImageTab.hpp"
#include <iostream>

const float ImageTab::TAB_WIDTH = 200.0f;
const float ImageTab::TAB_HEIGHT = 30.0f;
sf::Font ImageTab::font;
bool ImageTab::fontLoaded = false;

ImageTab::ImageTab(const std::string& name, size_t index) 
    : imageName(name), imageIndex(index) {
    
    if (!fontLoaded) {
        if (!font.loadFromFile("src/assets/fonts/DejaVuSans-Bold.ttf")) {
            std::cerr << "Erreur: Impossible de charger la police pour les onglets !" << std::endl;
            fontLoaded = false;
        } else {
            fontLoaded = true;
        }
    }
    
    if (fontLoaded) {
        tabText.setFont(font);
        closeButtonText.setFont(font);
    }
    tabText.setCharacterSize(12);
    tabText.setFillColor(sf::Color(220, 220, 220));
    
    modifiedIndicator.setRadius(3);
    modifiedIndicator.setFillColor(sf::Color(255, 165, 0));
    
    closeButtonText.setCharacterSize(14);
    closeButtonText.setString("X");
    closeButtonText.setFillColor(sf::Color(200, 200, 200));
    
    closeButton.setSize({16, 16});
    closeButton.setFillColor(sf::Color::Transparent);
}

void ImageTab::draw(sf::RenderWindow& window, const sf::Vector2f& position, bool isActive) {
    updateLayout(position);
    
    sf::Color tabColor = isActive ? sf::Color(60, 60, 65) : sf::Color(45, 45, 48);
    sf::Color borderColor = isActive ? sf::Color(100, 100, 105) : sf::Color(70, 70, 75);
    
    tabShape.setFillColor(tabColor);
    tabShape.setOutlineColor(borderColor);
    tabShape.setOutlineThickness(1);
    
    window.draw(tabShape);
    window.draw(tabText);
    
    if (isModified) {
        window.draw(modifiedIndicator);
    }
    
    closeButton.setFillColor(sf::Color(80, 80, 85, 100));
    window.draw(closeButton);
    window.draw(closeButtonText);
}

void ImageTab::updateLayout(const sf::Vector2f& position) {
    bounds = sf::FloatRect(position.x, position.y, TAB_WIDTH, TAB_HEIGHT);
    
    tabShape.setPosition(position);
    tabShape.setSize({TAB_WIDTH, TAB_HEIGHT});
    
    std::string displayName = imageName;
    if (displayName.length() > 20) {
        displayName = displayName.substr(0, 17) + "...";
    }
    tabText.setString(displayName);
    
    sf::FloatRect textBounds = tabText.getLocalBounds();
    float textX = position.x + 10;
    float textY = position.y + (TAB_HEIGHT - textBounds.height) / 2 - textBounds.top;
    tabText.setPosition(textX, textY);
    
    modifiedIndicator.setPosition(position.x + TAB_WIDTH - 40, position.y + TAB_HEIGHT / 2 - 3);
    
    float closeX = position.x + TAB_WIDTH - 20;
    float closeY = position.y + (TAB_HEIGHT - 16) / 2;
    closeButton.setPosition(closeX, closeY);
    closeButtonBounds = sf::FloatRect(closeX, closeY, 16, 16);
    
    sf::FloatRect closeBounds = closeButtonText.getLocalBounds();
    closeButtonText.setPosition(
        closeX + (16 - closeBounds.width) / 2 - closeBounds.left,
        closeY + (16 - closeBounds.height) / 2 - closeBounds.top
    );
}
