#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <functional>

struct ImageFormat {
    std::string name;
    int width;
    int height;
    ImageFormat(const std::string& n, int w, int h) : name(n), width(w), height(h) {}
};

class NewImageDialog {
public:
    NewImageDialog();
    void show();
    void hide();
    bool isVisible() const { return visible; }
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event);
    void setOnConfirm(std::function<void(int width, int height, const std::string& name)> callback);
    
private:
    void setupUI();
    void updatePresetsList();
    void updateInputFields();
    void onPresetSelected(int index);
    void onConfirmClicked();
    void onCancelClicked();
    void handleTextInput(sf::Uint32 unicode);
    void selectNextField();
    void selectPreviousField();
    bool visible = false;
    std::vector<ImageFormat> presets;
    int selectedPresetIndex = -1;
    enum InputField { INPUT_NAME, INPUT_WIDTH, INPUT_HEIGHT, INPUT_NONE };
    InputField activeField = INPUT_NAME;
    std::string imageName = "Untitled";
    std::string widthText = "800";
    std::string heightText = "600";
    sf::RectangleShape background;
    sf::RectangleShape dialogBox;
    sf::Text titleText;
    sf::Text nameLabel, widthLabel, heightLabel, presetsLabel;
    sf::Text nameInput, widthInput, heightInput;
    sf::RectangleShape confirmButton, cancelButton;
    sf::Text confirmText, cancelText;
    std::vector<sf::RectangleShape> presetButtons;
    std::vector<sf::Text> presetTexts;
    sf::RectangleShape nameField, widthField, heightField;
    sf::RectangleShape cursor;
    bool showCursor = true;
    sf::Clock cursorClock;
    static sf::Font font;
    static bool fontLoaded;
    std::function<void(int, int, const std::string&)> onConfirmCallback;
    static const float DIALOG_WIDTH;
    static const float DIALOG_HEIGHT;
    static const float FIELD_HEIGHT;
    static const float BUTTON_WIDTH;
    static const float BUTTON_HEIGHT;
};
