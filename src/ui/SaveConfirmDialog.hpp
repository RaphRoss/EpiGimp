#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

class SaveConfirmDialog {
public:
    SaveConfirmDialog();
    
    void show(const std::string& imageName);
    void hide();
    bool isVisible() const { return visible; }
    
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event);
    
    void setOnSave(std::function<void()> callback) { onSave = callback; }
    void setOnDontSave(std::function<void()> callback) { onDontSave = callback; }
    void setOnCancel(std::function<void()> callback) { onCancel = callback; }
    
private:
    void updateLayout();
    void setupButtons();
    
    bool visible = false;
    std::string currentImageName;
    
    sf::RectangleShape overlay;
    sf::RectangleShape dialogBox;
    sf::Text titleText;
    sf::Text messageText;
    
    sf::RectangleShape saveButton;
    sf::Text saveButtonText;
    sf::RectangleShape dontSaveButton;
    sf::Text dontSaveButtonText;
    sf::RectangleShape cancelButton;
    sf::Text cancelButtonText;
    
    std::function<void()> onSave;
    std::function<void()> onDontSave;
    std::function<void()> onCancel;
    
    static sf::Font font;
    static bool fontLoaded;
    
    static const float DIALOG_WIDTH;
    static const float DIALOG_HEIGHT;
    static const float BUTTON_WIDTH;
    static const float BUTTON_HEIGHT;
};
