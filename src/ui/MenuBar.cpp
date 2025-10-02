#include "MenuBar.hpp"

const float MenuBar::MENU_BAR_HEIGHT = 30.0f;

MenuBar::MenuBar() {
    background.setPosition(0, 0);
    background.setSize({800, MENU_BAR_HEIGHT});
    background.setFillColor(sf::Color(37, 37, 38));
    background.setOutlineColor(sf::Color(63, 63, 70));
    background.setOutlineThickness(1);
}

void MenuBar::addMenu(const std::string& title) {
    float xPos = menus.size() * 120.0f;
    auto menu = std::make_unique<Menu>(sf::Vector2f(xPos, 0), title);
    menus.push_back(std::move(menu));
}

void MenuBar::addMenuItem(const std::string& menuTitle, const std::string& itemLabel, std::function<void()> callback) {
    for (auto& menu : menus) {
        if (menu->getTitle() == menuTitle) {
            menu->addItem(itemLabel, callback);
            break;
        }
    }
}

void MenuBar::updateMenuPositions() {
    for (size_t i = 0; i < menus.size(); ++i) {
    }
}

void MenuBar::draw(sf::RenderWindow& window) {
    window.draw(background);
    for (auto& menu : menus) {
        menu->draw(window);
    }
}

bool MenuBar::handleClick(const sf::Vector2f& mousePos) {
    bool clickedOnMenu = false;
    for (auto& menu : menus) {
        if (menu->getTitleBounds().contains(mousePos) || 
            (menu->isOpen() && mousePos.y > MENU_BAR_HEIGHT && mousePos.x >= menu->getTitleBounds().left && 
             mousePos.x <= menu->getTitleBounds().left + 120)) {
            menu->handleClick(mousePos);
            clickedOnMenu = true;
        }
    }
    if (!clickedOnMenu && mousePos.y <= MENU_BAR_HEIGHT) {
        closeAllMenus();
    }
    return clickedOnMenu || mousePos.y <= MENU_BAR_HEIGHT;
}

void MenuBar::closeAllMenus() {
    for (auto& menu : menus) {
        menu->setOpen(false);
    }
}

void MenuBar::handleMouseMove(const sf::Vector2f& mousePos) {
    for (auto& menu : menus) {
        menu->handleMouseMove(mousePos);
    }
}
