#include "TabManager.hpp"
#include <iostream>
#include <algorithm>

const float TabManager::TAB_BAR_HEIGHT = 32.0f;

TabManager::TabManager() {
    background.setFillColor(sf::Color(37, 37, 38));
    background.setOutlineColor(sf::Color(63, 63, 70));
    background.setOutlineThickness(1);
    background.setPosition(220, 30);
    background.setSize({1700, TAB_BAR_HEIGHT});
}

void TabManager::addTab(const std::string& imageName, size_t imageIndex) {
    auto tab = std::make_unique<ImageTab>(imageName, imageIndex);
    tabs.push_back(std::move(tab));
    activeTabIndex = tabs.size() - 1;
    updateTabLayout();
}

void TabManager::removeTab(size_t tabIndex) {
    if (tabIndex >= tabs.size()) return;
    
    tabs.erase(tabs.begin() + tabIndex);
    
    if (tabs.empty()) {
        activeTabIndex = 0;
    } else if (activeTabIndex >= tabs.size()) {
        activeTabIndex = tabs.size() - 1;
    } else if (tabIndex <= activeTabIndex && activeTabIndex > 0) {
        activeTabIndex--;
    }
    
    updateTabLayout();
}

void TabManager::setActiveTab(size_t tabIndex) {
    if (tabIndex < tabs.size()) {
        activeTabIndex = tabIndex;
        if (onTabSelected) {
            onTabSelected(tabs[activeTabIndex]->getImageIndex());
        }
    }
}

void TabManager::setTabModified(size_t imageIndex, bool modified) {
    size_t tabIndex = findTabByImageIndex(imageIndex);
    if (tabIndex < tabs.size()) {
        tabs[tabIndex]->setModified(modified);
    }
}

void TabManager::updateTabName(size_t imageIndex, const std::string& newName) {
    size_t tabIndex = findTabByImageIndex(imageIndex);
    if (tabIndex < tabs.size()) {
        tabs[tabIndex]->setName(newName);
    }
}

void TabManager::draw(sf::RenderWindow& window) {
    window.draw(background);
    
    for (size_t i = 0; i < tabs.size(); ++i) {
        sf::Vector2f tabPosition(220 + i * ImageTab::TAB_WIDTH, 30);
        bool isActive = (i == activeTabIndex);
        tabs[i]->draw(window, tabPosition, isActive);
    }
}

bool TabManager::handleClick(const sf::Vector2f& mousePos) {
    for (size_t i = 0; i < tabs.size(); ++i) {
        sf::Vector2f tabPosition(220 + i * ImageTab::TAB_WIDTH, 30);
        
        if (tabs[i]->getCloseButtonBounds().contains(mousePos)) {
            if (onTabCloseRequested) {
                onTabCloseRequested(i);
            }
            return true;
        }
        
        if (tabs[i]->getBounds().contains(mousePos)) {
            setActiveTab(i);
            return true;
        }
    }
    return false;
}

void TabManager::handleMouseMove(const sf::Vector2f& mousePos) {
    hoveredTabIndex = -1;
    hoveredCloseButton = false;
    
    for (size_t i = 0; i < tabs.size(); ++i) {
        if (tabs[i]->getBounds().contains(mousePos)) {
            hoveredTabIndex = static_cast<int>(i);
            if (tabs[i]->getCloseButtonBounds().contains(mousePos)) {
                hoveredCloseButton = true;
            }
            break;
        }
    }
}

bool TabManager::hasModifiedTabs() const {
    return std::any_of(tabs.begin(), tabs.end(), 
        [](const std::unique_ptr<ImageTab>& tab) {
            return tab->getModified();
        });
}

void TabManager::updateTabLayout() {
}

size_t TabManager::findTabByImageIndex(size_t imageIndex) {
    for (size_t i = 0; i < tabs.size(); ++i) {
        if (tabs[i]->getImageIndex() == imageIndex) {
            return i;
        }
    }
    return tabs.size();
}
