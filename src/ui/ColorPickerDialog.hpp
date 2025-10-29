#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

class ColorPickerDialog {
public:
    ColorPickerDialog();
    void show(bool editForeground);
    void hide();
    bool isVisible() const { return visible; }
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);

private:
    void updateFromHSV();
    void updateFromRGBA();
    sf::Color getCurrentColor() const;
    void setCurrentColor(const sf::Color& c);
    void applyColor();
    void drawHistory(sf::RenderWindow& window);
    bool handleHistoryClick(const sf::Vector2f& pos);

    bool visible = false;
    bool editingForeground = true;
    sf::RectangleShape backdrop;
    sf::RectangleShape panel;
    sf::RectangleShape hueBar;
    sf::RectangleShape svBox;
    sf::RectangleShape okButton;
    sf::Text okText;
    sf::Text title;
    sf::Text rgbText[4];
    sf::RectangleShape rgbBars[4];
    float hue = 0.f;
    float sat = 0.f;
    float val = 1.f;
    int rgba[4] = {0,0,0,255};
    bool draggingHue = false;
    bool draggingSV = false;
    int draggingRGBA = -1;
    static sf::Font font;
    static bool fontLoaded;
    std::vector<sf::RectangleShape> historyRects;
};
