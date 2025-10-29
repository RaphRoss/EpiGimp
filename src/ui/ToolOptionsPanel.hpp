#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class ToolOptionsPanel {
public:
    ToolOptionsPanel();
    void setPosition(const sf::Vector2f& pos);
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event);

    void configure(const std::string& title,
                   const std::string& label1, float min1, float max1, float value1, std::function<void(float)> cb1,
                   const std::string& label2, float min2, float max2, float value2, std::function<void(float)> cb2,
                   const std::string& label3, float min3, float max3, float value3, std::function<void(float)> cb3);

private:
    struct Slider {
        sf::Text label;
        sf::RectangleShape bar;
        sf::RectangleShape knob;
        float min=0, max=1, value=0;
        std::function<void(float)> onChange;
        bool dragging=false;
    };
    sf::RectangleShape background;
    sf::Text titleText;
    Slider s1, s2, s3;
    static sf::Font font;
    static bool fontLoaded;
};
