#include "NewImageDialog.hpp"
#include <iostream>
#include <sstream>

sf::Font NewImageDialog::font;
bool NewImageDialog::fontLoaded = false;
const float NewImageDialog::DIALOG_WIDTH = 500.0f;
const float NewImageDialog::DIALOG_HEIGHT = 400.0f;
const float NewImageDialog::FIELD_HEIGHT = 25.0f;
const float NewImageDialog::BUTTON_WIDTH = 80.0f;
const float NewImageDialog::BUTTON_HEIGHT = 30.0f;

NewImageDialog::NewImageDialog() {
    if (!fontLoaded) {
        if (!font.loadFromFile("src/assets/fonts/DejaVuSans-Bold.ttf")) {
            std::cerr << "Erreur: Impossible de charger la police pour NewImageDialog !" << std::endl;
            fontLoaded = false;
        } else {
            fontLoaded = true;
        }
    }
    
    presets.emplace_back("A4 (Portrait)", 2480, 3508);
    presets.emplace_back("A4 (Landscape)", 3508, 2480);
    presets.emplace_back("A3 (Portrait)", 3508, 4961);
    presets.emplace_back("A3 (Landscape)", 4961, 3508);
    presets.emplace_back("A2 (Portrait)", 4961, 7016);
    presets.emplace_back("A2 (Landscape)", 7016, 4961);
    presets.emplace_back("A1 (Portrait)", 7016, 9933);
    presets.emplace_back("A1 (Landscape)", 9933, 7016);
    
    setupUI();
}

void NewImageDialog::setupUI() {
    background.setSize({1920, 1080});
    background.setFillColor(sf::Color(0, 0, 0, 180));
    dialogBox.setSize({DIALOG_WIDTH, DIALOG_HEIGHT});
    dialogBox.setPosition({(1920 - DIALOG_WIDTH) / 2, (1080 - DIALOG_HEIGHT) / 2});
    dialogBox.setFillColor(sf::Color(37, 37, 38));
    dialogBox.setOutlineColor(sf::Color(63, 63, 70));
    dialogBox.setOutlineThickness(2);
    
    sf::Vector2f dialogPos = dialogBox.getPosition();
    if (fontLoaded) {
        titleText.setFont(font);
    }
    titleText.setString("Create New Image");
    titleText.setCharacterSize(18);
    titleText.setFillColor(sf::Color(204, 204, 204));
    titleText.setPosition(dialogPos.x + 20, dialogPos.y + 10);
    float yOffset = 50;
    
    if (fontLoaded) {
        nameLabel.setFont(font);
        nameInput.setFont(font);
    }
    nameLabel.setString("Name:");
    nameLabel.setCharacterSize(14);
    nameLabel.setFillColor(sf::Color(204, 204, 204));
    nameLabel.setPosition(dialogPos.x + 20, dialogPos.y + yOffset);
    nameField.setSize({200, FIELD_HEIGHT});
    nameField.setPosition(dialogPos.x + 100, dialogPos.y + yOffset - 2);
    nameField.setFillColor(sf::Color(51, 51, 55));
    nameField.setOutlineColor(sf::Color(63, 63, 70));
    nameField.setOutlineThickness(1);
    nameInput.setString(imageName);
    nameInput.setCharacterSize(12);
    nameInput.setFillColor(sf::Color(204, 204, 204));
    nameInput.setPosition(dialogPos.x + 105, dialogPos.y + yOffset + 2);
    
    yOffset += 40;
    if (fontLoaded) {
        widthLabel.setFont(font);
        widthInput.setFont(font);
    }
    widthLabel.setString("Width (px):");
    widthLabel.setCharacterSize(14);
    widthLabel.setFillColor(sf::Color(204, 204, 204));
    widthLabel.setPosition(dialogPos.x + 20, dialogPos.y + yOffset);
    widthField.setSize({100, FIELD_HEIGHT});
    widthField.setPosition(dialogPos.x + 120, dialogPos.y + yOffset - 2);
    widthField.setFillColor(sf::Color(51, 51, 55));
    widthField.setOutlineColor(sf::Color(63, 63, 70));
    widthField.setOutlineThickness(1);
    widthInput.setString(widthText);
    widthInput.setCharacterSize(12);
    widthInput.setFillColor(sf::Color(204, 204, 204));
    widthInput.setPosition(dialogPos.x + 125, dialogPos.y + yOffset + 2);
    
    if (fontLoaded) {
        heightLabel.setFont(font);
        heightInput.setFont(font);
    }
    heightLabel.setString("Height (px):");
    heightLabel.setCharacterSize(14);
    heightLabel.setFillColor(sf::Color(204, 204, 204));
    heightLabel.setPosition(dialogPos.x + 250, dialogPos.y + yOffset);
    heightField.setSize({100, FIELD_HEIGHT});
    heightField.setPosition(dialogPos.x + 350, dialogPos.y + yOffset - 2);
    heightField.setFillColor(sf::Color(51, 51, 55));
    heightField.setOutlineColor(sf::Color(63, 63, 70));
    heightField.setOutlineThickness(1);
    heightInput.setString(heightText);
    heightInput.setCharacterSize(12);
    heightInput.setFillColor(sf::Color(204, 204, 204));
    heightInput.setPosition(dialogPos.x + 355, dialogPos.y + yOffset + 2);
    
    yOffset += 50;
    if (fontLoaded) {
        presetsLabel.setFont(font);
    }
    presetsLabel.setString("Presets:");
    presetsLabel.setCharacterSize(14);
    presetsLabel.setFillColor(sf::Color(204, 204, 204));
    presetsLabel.setPosition(dialogPos.x + 20, dialogPos.y + yOffset);
    updatePresetsList();
    float buttonY = dialogPos.y + DIALOG_HEIGHT - 50;
    confirmButton.setSize({BUTTON_WIDTH, BUTTON_HEIGHT});
    confirmButton.setPosition(dialogPos.x + DIALOG_WIDTH - 2 * BUTTON_WIDTH - 30, buttonY);
    confirmButton.setFillColor(sf::Color(0, 122, 204));
    confirmButton.setOutlineColor(sf::Color(63, 63, 70));
    confirmButton.setOutlineThickness(1);
    if (fontLoaded) {
        confirmText.setFont(font);
    }
    confirmText.setString("OK");
    confirmText.setCharacterSize(14);
    confirmText.setFillColor(sf::Color(255, 255, 255));
    sf::FloatRect confirmBounds = confirmText.getLocalBounds();
    confirmText.setPosition(
        confirmButton.getPosition().x + (BUTTON_WIDTH - confirmBounds.width) / 2,
        confirmButton.getPosition().y + (BUTTON_HEIGHT - confirmBounds.height) / 2
    );
    
    cancelButton.setSize({BUTTON_WIDTH, BUTTON_HEIGHT});
    cancelButton.setPosition(dialogPos.x + DIALOG_WIDTH - BUTTON_WIDTH - 20, buttonY);
    cancelButton.setFillColor(sf::Color(51, 51, 55));
    cancelButton.setOutlineColor(sf::Color(63, 63, 70));
    cancelButton.setOutlineThickness(1);
    if (fontLoaded) {
        cancelText.setFont(font);
    }
    cancelText.setString("Cancel");
    cancelText.setCharacterSize(14);
    cancelText.setFillColor(sf::Color(204, 204, 204));
    sf::FloatRect cancelBounds = cancelText.getLocalBounds();
    cancelText.setPosition(
        cancelButton.getPosition().x + (BUTTON_WIDTH - cancelBounds.width) / 2,
        cancelButton.getPosition().y + (BUTTON_HEIGHT - cancelBounds.height) / 2
    );
    
    cursor.setSize({1, FIELD_HEIGHT - 4});
    cursor.setFillColor(sf::Color(204, 204, 204));
}

void NewImageDialog::updatePresetsList() {
    sf::Vector2f dialogPos = dialogBox.getPosition();
    float yOffset = 170;
    
    presetButtons.clear();
    presetTexts.clear();
    
    for (size_t i = 0; i < presets.size(); ++i) {
        sf::RectangleShape button;
        button.setSize({DIALOG_WIDTH - 40, 25});
        button.setPosition(dialogPos.x + 20, dialogPos.y + yOffset + i * 27);
        button.setFillColor(i == static_cast<size_t>(selectedPresetIndex) ? 
                           sf::Color(0, 122, 204) : sf::Color(51, 51, 55));
        button.setOutlineColor(sf::Color(63, 63, 70));
        button.setOutlineThickness(1);
        presetButtons.push_back(button);
        
        sf::Text text;
        if (fontLoaded) {
            text.setFont(font);
        }
        std::stringstream ss;
        ss << presets[i].name << " (" << presets[i].width << "x" << presets[i].height << ")";
        text.setString(ss.str());
        text.setCharacterSize(12);
        text.setFillColor(sf::Color(204, 204, 204));
        text.setPosition(dialogPos.x + 25, dialogPos.y + yOffset + i * 27 + 4);
        presetTexts.push_back(text);
    }
}

void NewImageDialog::show() {
    visible = true;
    activeField = INPUT_NAME;
}

void NewImageDialog::hide() {
    visible = false;
}

void NewImageDialog::draw(sf::RenderWindow& window) {
    if (!visible) return;
    
    window.draw(background);
    window.draw(dialogBox);
    
    if (fontLoaded) {
        window.draw(titleText);
        window.draw(nameLabel);
        window.draw(widthLabel);
        window.draw(heightLabel);
        window.draw(presetsLabel);
    }
    
    window.draw(nameField);
    window.draw(widthField);
    window.draw(heightField);
    
    if (fontLoaded) {
        window.draw(nameInput);
        window.draw(widthInput);
        window.draw(heightInput);
    }
    
    for (size_t i = 0; i < presetButtons.size(); ++i) {
        window.draw(presetButtons[i]);
        if (fontLoaded) {
            window.draw(presetTexts[i]);
        }
    }
    
    window.draw(confirmButton);
    window.draw(cancelButton);
    if (fontLoaded) {
        window.draw(confirmText);
        window.draw(cancelText);
    }
    
    if (showCursor && cursorClock.getElapsedTime().asMilliseconds() < 500) {
        sf::Vector2f cursorPos;
        switch (activeField) {
            case INPUT_NAME:
                cursorPos = nameInput.getPosition();
                cursorPos.x += nameInput.getLocalBounds().width + 2;
                cursorPos.y -= 2;
                break;
            case INPUT_WIDTH:
                cursorPos = widthInput.getPosition();
                cursorPos.x += widthInput.getLocalBounds().width + 2;
                cursorPos.y -= 2;
                break;
            case INPUT_HEIGHT:
                cursorPos = heightInput.getPosition();
                cursorPos.x += heightInput.getLocalBounds().width + 2;
                cursorPos.y -= 2;
                break;
            default:
                return;
        }
        cursor.setPosition(cursorPos);
        window.draw(cursor);
    }
    
    if (cursorClock.getElapsedTime().asMilliseconds() > 1000) {
        cursorClock.restart();
    }
}

void NewImageDialog::handleEvent(const sf::Event& event) {
    if (!visible) return;
    
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        
        if (confirmButton.getGlobalBounds().contains(mousePos)) {
            onConfirmClicked();
            return;
        }
        
        if (cancelButton.getGlobalBounds().contains(mousePos)) {
            onCancelClicked();
            return;
        }
        
        if (nameField.getGlobalBounds().contains(mousePos)) {
            activeField = INPUT_NAME;
        } else if (widthField.getGlobalBounds().contains(mousePos)) {
            activeField = INPUT_WIDTH;
        } else if (heightField.getGlobalBounds().contains(mousePos)) {
            activeField = INPUT_HEIGHT;
        } else {
            activeField = INPUT_NONE;
        }
        
        for (size_t i = 0; i < presetButtons.size(); ++i) {
            if (presetButtons[i].getGlobalBounds().contains(mousePos)) {
                onPresetSelected(static_cast<int>(i));
                break;
            }
        }
    }
    
    if (event.type == sf::Event::TextEntered) {
        handleTextInput(event.text.unicode);
    }
    
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Tab) {
            selectNextField();
        } else if (event.key.code == sf::Keyboard::Enter) {
            onConfirmClicked();
        } else if (event.key.code == sf::Keyboard::Escape) {
            onCancelClicked();
        }
    }
}

void NewImageDialog::handleTextInput(sf::Uint32 unicode) {
    if (activeField == INPUT_NONE) return;
    
    std::string* targetString = nullptr;
    sf::Text* targetText = nullptr;
    
    switch (activeField) {
        case INPUT_NAME:
            targetString = &imageName;
            targetText = &nameInput;
            break;
        case INPUT_WIDTH:
            targetString = &widthText;
            targetText = &widthInput;
            break;
        case INPUT_HEIGHT:
            targetString = &heightText;
            targetText = &heightInput;
            break;
        default:
            return;
    }
    
    if (unicode == 8) {
        if (!targetString->empty()) {
            targetString->pop_back();
            targetText->setString(*targetString);
        }
    } else if (unicode >= 32 && unicode < 127) {
        if ((activeField == INPUT_WIDTH || activeField == INPUT_HEIGHT) && 
            (unicode < '0' || unicode > '9')) {
            return;
        }
        
        targetString->push_back(static_cast<char>(unicode));
        targetText->setString(*targetString);
    }
}

void NewImageDialog::onPresetSelected(int index) {
    if (index >= 0 && index < static_cast<int>(presets.size())) {
        selectedPresetIndex = index;
        widthText = std::to_string(presets[index].width);
        heightText = std::to_string(presets[index].height);
        
        widthInput.setString(widthText);
        heightInput.setString(heightText);
        
        updatePresetsList();
    }
}

void NewImageDialog::selectNextField() {
    switch (activeField) {
        case INPUT_NAME:
            activeField = INPUT_WIDTH;
            break;
        case INPUT_WIDTH:
            activeField = INPUT_HEIGHT;
            break;
        case INPUT_HEIGHT:
            activeField = INPUT_NAME;
            break;
        case INPUT_NONE:
            activeField = INPUT_NAME;
            break;
    }
}

void NewImageDialog::onConfirmClicked() {
    try {
        int width = std::stoi(widthText);
        int height = std::stoi(heightText);
        
        if (width <= 0 || height <= 0) {
            std::cerr << "Erreur: Les dimensions doivent être positives!" << std::endl;
            return;
        }
        
        if (width > 10000 || height > 10000) {
            std::cerr << "Erreur: Les dimensions sont trop grandes!" << std::endl;
            return;
        }
        
        if (onConfirmCallback) {
            onConfirmCallback(width, height, imageName);
        }
        
        hide();
    } catch (const std::exception& e) {
        std::cerr << "Erreur: Dimensions invalides!" << std::endl;
    }
}

void NewImageDialog::onCancelClicked() {
    hide();
}

void NewImageDialog::setOnConfirm(std::function<void(int, int, const std::string&)> callback) {
    onConfirmCallback = callback;
}
