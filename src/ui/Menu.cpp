#include "Menu.hpp"
#include <iostream>

sf::Font Menu::font;
bool Menu::fontLoaded = false;
const float Menu::ITEM_HEIGHT = 25.0f;
const float Menu::MENU_WIDTH = 120.0f;

Menu::Menu(const sf::Vector2f& pos, const std::string& title) : position(pos), title(title) {
    if (!fontLoaded) {
        if (!font.loadFromFile("src/assets/fonts/DejaVuSans-Bold.ttf")) {
            std::cerr << "Erreur: Impossible de charger la police pour le menu !" << std::endl;
            fontLoaded = false;
        } else {
            fontLoaded = true;
        }
    }
    
    titleRect.setPosition(position);
    titleRect.setSize({MENU_WIDTH, ITEM_HEIGHT});
    titleRect.setFillColor(sf::Color(37, 37, 38));
    titleRect.setOutlineColor(sf::Color(63, 63, 70));
    titleRect.setOutlineThickness(1);
    if (fontLoaded) {
        titleText.setFont(font);
    }
    titleText.setString(title);
    titleText.setCharacterSize(14);
    titleText.setFillColor(sf::Color(204, 204, 204));
    
    sf::FloatRect bounds = titleText.getLocalBounds();
    titleText.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
    titleText.setPosition(position.x + MENU_WIDTH / 2, position.y + ITEM_HEIGHT / 2);
}

void Menu::addItem(const std::string& label, std::function<void()> callback) {
    items.emplace_back(label, callback);
    updateLayout();
}

void Menu::updateLayout() {
    itemRects.clear();
    itemTexts.clear();
    
    for (size_t i = 0; i < items.size(); ++i) {
        sf::RectangleShape rect;
        rect.setPosition(position.x, position.y + ITEM_HEIGHT * (i + 1));
        rect.setSize({MENU_WIDTH, ITEM_HEIGHT});
        rect.setFillColor(sf::Color(51, 51, 55));
        rect.setOutlineColor(sf::Color(63, 63, 70));
        rect.setOutlineThickness(1);
        itemRects.push_back(rect);
        sf::Text text;
        if (fontLoaded) {
            text.setFont(font);
        }
        text.setString(items[i].label);
        text.setCharacterSize(12);
        text.setFillColor(sf::Color(204, 204, 204));
        
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            position.x + 5, 
            position.y + ITEM_HEIGHT * (i + 1) + (ITEM_HEIGHT - textBounds.height) / 2 - textBounds.top
        );
        itemTexts.push_back(text);
    }
}

void Menu::draw(sf::RenderWindow& window) {
    window.draw(titleRect);
    if (fontLoaded) {
        window.draw(titleText);
    }
    if (open && !itemRects.empty()) {
        sf::RectangleShape dropdownBackground;
        dropdownBackground.setPosition(position.x - 2, position.y + ITEM_HEIGHT - 2);
        dropdownBackground.setSize({MENU_WIDTH + 4, ITEM_HEIGHT * items.size() + 4});
        dropdownBackground.setFillColor(sf::Color(37, 37, 38, 240));
        dropdownBackground.setOutlineColor(sf::Color(63, 63, 70));
        dropdownBackground.setOutlineThickness(2);
        window.draw(dropdownBackground);
        for (size_t i = 0; i < itemRects.size(); ++i) {
            window.draw(itemRects[i]);
            if (fontLoaded) {
                window.draw(itemTexts[i]);
            }
        }
    }
}

void Menu::handleClick(const sf::Vector2f& mousePos) {
    if (titleRect.getGlobalBounds().contains(mousePos)) {
        toggleOpen();
        return;
    }
    
    if (open) {
        for (size_t i = 0; i < itemRects.size(); ++i) {
            if (itemRects[i].getGlobalBounds().contains(mousePos)) {
                if (items[i].callback) {
                    items[i].callback();
                }
                setOpen(false);
                return;
            }
        }
    }
}

sf::FloatRect Menu::getTitleBounds() const {
    return titleRect.getGlobalBounds();
}

void Menu::handleMouseMove(const sf::Vector2f& mousePos) {
    hoveredItemIndex = -1;
    
    if (open) {
        for (size_t i = 0; i < itemRects.size(); ++i) {
            if (itemRects[i].getGlobalBounds().contains(mousePos)) {
                hoveredItemIndex = static_cast<int>(i);
                break;
            }
        }
        
        // Mettre à jour les couleurs en fonction du survol
        for (size_t i = 0; i < itemRects.size(); ++i) {
            if (static_cast<int>(i) == hoveredItemIndex) {
                itemRects[i].setFillColor(sf::Color(0, 122, 204)); // Bleu pour le survol
            } else {
                itemRects[i].setFillColor(sf::Color(51, 51, 55)); // Couleur normale
            }
        }
    }
}
