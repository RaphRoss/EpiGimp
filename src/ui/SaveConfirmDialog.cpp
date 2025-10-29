#include "SaveConfirmDialog.hpp"
#include <iostream>

const float SaveConfirmDialog::DIALOG_WIDTH = 450.0f;
const float SaveConfirmDialog::DIALOG_HEIGHT = 180.0f;
const float SaveConfirmDialog::BUTTON_WIDTH = 120.0f;
const float SaveConfirmDialog::BUTTON_HEIGHT = 35.0f;

sf::Font SaveConfirmDialog::font;
bool SaveConfirmDialog::fontLoaded = false;

SaveConfirmDialog::SaveConfirmDialog() {
    if (!fontLoaded) {
        if (!font.loadFromFile("src/assets/fonts/DejaVuSans-Bold.ttf")) {
            std::cerr << "Error: Unable to load font for dialog box!" << std::endl;
            fontLoaded = false;
        } else {
            fontLoaded = true;
        }
    }
    
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    
    dialogBox.setFillColor(sf::Color(60, 60, 65));
    dialogBox.setOutlineColor(sf::Color(100, 100, 105));
    dialogBox.setOutlineThickness(2);
    
    if (fontLoaded) {
        titleText.setFont(font);
        messageText.setFont(font);
        saveButtonText.setFont(font);
        dontSaveButtonText.setFont(font);
        cancelButtonText.setFont(font);
    }
    
    titleText.setCharacterSize(16);
    titleText.setFillColor(sf::Color::White);
    titleText.setString("Save changes?");
    
    messageText.setCharacterSize(12);
    messageText.setFillColor(sf::Color(220, 220, 220));
    
    setupButtons();
}

void SaveConfirmDialog::setupButtons() {
    saveButton.setSize({BUTTON_WIDTH, BUTTON_HEIGHT});
    saveButton.setFillColor(sf::Color(0, 120, 215));
    saveButton.setOutlineColor(sf::Color(100, 160, 235));
    saveButton.setOutlineThickness(1);
    
    saveButtonText.setCharacterSize(12);
    saveButtonText.setFillColor(sf::Color::White);
    saveButtonText.setString("Save");
    
    dontSaveButton.setSize({BUTTON_WIDTH, BUTTON_HEIGHT});
    dontSaveButton.setFillColor(sf::Color(80, 80, 85));
    dontSaveButton.setOutlineColor(sf::Color(120, 120, 125));
    dontSaveButton.setOutlineThickness(1);
    
    dontSaveButtonText.setCharacterSize(12);
    dontSaveButtonText.setFillColor(sf::Color::White);
    dontSaveButtonText.setString("Don't Save");
    
    cancelButton.setSize({BUTTON_WIDTH, BUTTON_HEIGHT});
    cancelButton.setFillColor(sf::Color(80, 80, 85));
    cancelButton.setOutlineColor(sf::Color(120, 120, 125));
    cancelButton.setOutlineThickness(1);
    
    cancelButtonText.setCharacterSize(12);
    cancelButtonText.setFillColor(sf::Color::White);
    cancelButtonText.setString("Cancel");
}

void SaveConfirmDialog::show(const std::string& imageName) {
    visible = true;
    currentImageName = imageName;
    
    std::string message = "The image \"" + imageName + "\" has been modified.\n";
    message += "Do you want to save the changes before closing?";
    messageText.setString(message);
    
    updateLayout();
}

void SaveConfirmDialog::hide() {
    visible = false;
}

void SaveConfirmDialog::updateLayout() {
    float centerX = 960.0f;
    float centerY = 540.0f;
    
    overlay.setSize({1920, 1080});
    overlay.setPosition(0, 0);
    
    float dialogX = centerX - DIALOG_WIDTH / 2;
    float dialogY = centerY - DIALOG_HEIGHT / 2;
    dialogBox.setPosition(dialogX, dialogY);
    dialogBox.setSize({DIALOG_WIDTH, DIALOG_HEIGHT});
    
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(
        dialogX + (DIALOG_WIDTH - titleBounds.width) / 2,
        dialogY + 20 - titleBounds.top
    );
    
    sf::FloatRect messageBounds = messageText.getLocalBounds();
    messageText.setPosition(
        dialogX + (DIALOG_WIDTH - messageBounds.width) / 2,
        dialogY + 60 - messageBounds.top
    );
    
    float buttonY = dialogY + DIALOG_HEIGHT - BUTTON_HEIGHT - 20;
    float buttonSpacing = (DIALOG_WIDTH - 3 * BUTTON_WIDTH) / 4;
    
    float saveX = dialogX + buttonSpacing;
    saveButton.setPosition(saveX, buttonY);
    sf::FloatRect saveBounds = saveButtonText.getLocalBounds();
    saveButtonText.setPosition(
        saveX + (BUTTON_WIDTH - saveBounds.width) / 2 - saveBounds.left,
        buttonY + (BUTTON_HEIGHT - saveBounds.height) / 2 - saveBounds.top
    );
    
    float dontSaveX = saveX + BUTTON_WIDTH + buttonSpacing;
    dontSaveButton.setPosition(dontSaveX, buttonY);
    sf::FloatRect dontSaveBounds = dontSaveButtonText.getLocalBounds();
    dontSaveButtonText.setPosition(
        dontSaveX + (BUTTON_WIDTH - dontSaveBounds.width) / 2 - dontSaveBounds.left,
        buttonY + (BUTTON_HEIGHT - dontSaveBounds.height) / 2 - dontSaveBounds.top
    );
    
    float cancelX = dontSaveX + BUTTON_WIDTH + buttonSpacing;
    cancelButton.setPosition(cancelX, buttonY);
    sf::FloatRect cancelBounds = cancelButtonText.getLocalBounds();
    cancelButtonText.setPosition(
        cancelX + (BUTTON_WIDTH - cancelBounds.width) / 2 - cancelBounds.left,
        buttonY + (BUTTON_HEIGHT - cancelBounds.height) / 2 - cancelBounds.top
    );
}

void SaveConfirmDialog::draw(sf::RenderWindow& window) {
    if (!visible) return;
    
    window.draw(overlay);
    window.draw(dialogBox);
    window.draw(titleText);
    window.draw(messageText);
    window.draw(saveButton);
    window.draw(saveButtonText);
    window.draw(dontSaveButton);
    window.draw(dontSaveButtonText);
    window.draw(cancelButton);
    window.draw(cancelButtonText);
}

void SaveConfirmDialog::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (!visible) return;
    
    if (event.type == sf::Event::MouseButtonPressed) {
        // Use real mouse position from window to avoid titlebar offset issues
        sf::Vector2i realPixelPos = sf::Mouse::getPosition(window);
        // Apply vertical offset to compensate for window titlebar
        sf::Vector2f mousePos(static_cast<float>(realPixelPos.x), 
                             static_cast<float>(realPixelPos.y - 30));
        
        if (saveButton.getGlobalBounds().contains(mousePos)) {
            if (onSave) onSave();
            hide();
        } else if (dontSaveButton.getGlobalBounds().contains(mousePos)) {
            if (onDontSave) onDontSave();
            hide();
        } else if (cancelButton.getGlobalBounds().contains(mousePos)) {
            if (onCancel) onCancel();
            hide();
        }
    }
    
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            if (onCancel) onCancel();
            hide();
        } else if (event.key.code == sf::Keyboard::Enter) {
            if (onSave) onSave();
            hide();
        }
    }
}
