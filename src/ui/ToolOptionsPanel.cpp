#include "ToolOptionsPanel.hpp"
#include <algorithm>

sf::Font ToolOptionsPanel::font;
bool ToolOptionsPanel::fontLoaded = false;

ToolOptionsPanel::ToolOptionsPanel() {
    if (!fontLoaded) {
        if (!font.loadFromFile("src/assets/fonts/DejaVuSans-Bold.ttf")) fontLoaded=false; else fontLoaded=true;
    }
    background.setSize({200, 300});
    background.setFillColor(sf::Color(37,37,38));
    background.setOutlineColor(sf::Color(63,63,70));
    background.setOutlineThickness(1);
    if (fontLoaded) titleText.setFont(font);
    titleText.setCharacterSize(14);
    titleText.setFillColor(sf::Color(204,204,204));
    for (auto* s : {&s1,&s2,&s3}) {
        if (fontLoaded) s->label.setFont(font);
        s->label.setCharacterSize(12);
        s->label.setFillColor(sf::Color(204,204,204));
        s->bar.setSize({160, 6});
        s->bar.setFillColor(sf::Color(51,51,55));
        s->bar.setOutlineColor(sf::Color(63,63,70));
        s->bar.setOutlineThickness(1);
        s->knob.setSize({8,16});
        s->knob.setFillColor(sf::Color(0,122,204));
    }
    setPosition({10, 690});
}

void ToolOptionsPanel::setPosition(const sf::Vector2f& pos) {
    background.setPosition(pos);
    titleText.setPosition(pos.x + 10, pos.y + 10);
    float y = pos.y + 40;
    for (auto* s : {&s1,&s2,&s3}) {
        s->label.setPosition(pos.x + 10, y - 16);
        s->bar.setPosition(pos.x + 10, y);
        s->knob.setPosition(pos.x + 10, y - 5);
        y += 80;
    }
}

void ToolOptionsPanel::configure(const std::string& title,
                   const std::string& label1, float min1, float max1, float value1, std::function<void(float)> cb1,
                   const std::string& label2, float min2, float max2, float value2, std::function<void(float)> cb2,
                   const std::string& label3, float min3, float max3, float value3, std::function<void(float)> cb3) {
    titleText.setString(title);
    s1.label.setString(label1);
    s1.min=min1; s1.max=max1; s1.value=value1; s1.onChange=cb1;
    s2.label.setString(label2);
    s2.min=min2; s2.max=max2; s2.value=value2; s2.onChange=cb2;
    s3.label.setString(label3);
    s3.min=min3; s3.max=max3; s3.value=value3; s3.onChange=cb3;
    for (auto* s : {&s1,&s2,&s3}) {
        float t = (s->value - s->min) / (s->max - s->min);
        float x = s->bar.getPosition().x + t * s->bar.getSize().x;
        s->knob.setPosition(x - s->knob.getSize().x/2, s->knob.getPosition().y);
    }
}

void ToolOptionsPanel::draw(sf::RenderWindow& window) {
    window.draw(background);
    if (fontLoaded) window.draw(titleText);
    for (auto* s : {&s1,&s2,&s3}) {
        if (fontLoaded) window.draw(s->label);
        window.draw(s->bar);
        window.draw(s->knob);
    }
}

void ToolOptionsPanel::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f p(event.mouseButton.x, event.mouseButton.y);
        for (auto* s : {&s1,&s2,&s3}) {
            if (s->bar.getGlobalBounds().contains(p)) {
                s->dragging = true;
                float t = (p.x - s->bar.getPosition().x) / s->bar.getSize().x;
                t = std::max(0.f, std::min(1.f, t));
                s->value = s->min + t * (s->max - s->min);
                float x = s->bar.getPosition().x + t * s->bar.getSize().x;
                s->knob.setPosition(x - s->knob.getSize().x/2, s->knob.getPosition().y);
                if (s->onChange) s->onChange(s->value);
                break;
            }
        }
    }
    if (event.type == sf::Event::MouseButtonReleased) {
        for (auto* s : {&s1,&s2,&s3}) s->dragging=false;
    }
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f p(event.mouseMove.x, event.mouseMove.y);
        for (auto* s : {&s1,&s2,&s3}) {
            if (!s->dragging) continue;
            float t = (p.x - s->bar.getPosition().x) / s->bar.getSize().x;
            t = std::max(0.f, std::min(1.f, t));
            s->value = s->min + t * (s->max - s->min);
            float x = s->bar.getPosition().x + t * s->bar.getSize().x;
            s->knob.setPosition(x - s->knob.getSize().x/2, s->knob.getPosition().y);
            if (s->onChange) s->onChange(s->value);
        }
    }
}
