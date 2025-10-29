#include "ColorPickerDialog.hpp"
#include "../core/ColorManager.hpp"
#include <algorithm>
#include <cmath>

sf::Font ColorPickerDialog::font;
bool ColorPickerDialog::fontLoaded = false;

static sf::Color hsvToRgb(float h, float s, float v) {
    float c = v * s;
    float x = c * (1 - std::fabs(std::fmod(h / 60.0f, 2) - 1));
    float m = v - c;
    float r=0,g=0,b=0;
    int hi = static_cast<int>(h / 60.0f) % 6;
    switch (hi) {
        case 0: r=c; g=x; b=0; break;
        case 1: r=x; g=c; b=0; break;
        case 2: r=0; g=c; b=x; break;
        case 3: r=0; g=x; b=c; break;
        case 4: r=x; g=0; b=c; break;
        case 5: r=c; g=0; b=x; break;
    }
    sf::Uint8 R = static_cast<sf::Uint8>((r + m) * 255);
    sf::Uint8 G = static_cast<sf::Uint8>((g + m) * 255);
    sf::Uint8 B = static_cast<sf::Uint8>((b + m) * 255);
    return sf::Color(R,G,B);
}

static void rgbToHsv(const sf::Color& c, float& h, float& s, float& v) {
    float r = c.r / 255.f, g = c.g / 255.f, b = c.b / 255.f;
    float mx = std::max({r,g,b});
    float mn = std::min({r,g,b});
    float d = mx - mn;
    if (d == 0) h = 0;
    else if (mx == r) h = 60.f * std::fmod(((g - b) / d), 6.f);
    else if (mx == g) h = 60.f * (((b - r) / d) + 2.f);
    else h = 60.f * (((r - g) / d) + 4.f);
    if (h < 0) h += 360.f;
    s = mx == 0 ? 0 : d / mx;
    v = mx;
}

ColorPickerDialog::ColorPickerDialog() {
    if (!fontLoaded) {
        if (!font.loadFromFile("src/assets/fonts/DejaVuSans-Bold.ttf")) {
            fontLoaded = false;
        } else fontLoaded = true;
    }
    backdrop.setSize({10000,10000});
    backdrop.setFillColor(sf::Color(0,0,0,180));
    panel.setSize({520,360});
    panel.setPosition({700,360});
    panel.setFillColor(sf::Color(37,37,38));
    panel.setOutlineColor(sf::Color(63,63,70));
    panel.setOutlineThickness(2);
    hueBar.setSize({360,16});
    hueBar.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 40);
    svBox.setSize({360,200});
    svBox.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 70);
    okButton.setSize({80,30});
    okButton.setPosition(panel.getPosition().x + 420, panel.getPosition().y + 300);
    okButton.setFillColor(sf::Color(0,122,204));
    if (fontLoaded) {
        okText.setFont(font);
        title.setFont(font);
        for (int i=0;i<4;++i) rgbText[i].setFont(font);
    }
    okText.setString("OK");
    okText.setCharacterSize(14);
    okText.setFillColor(sf::Color::White);
    title.setString("Color Picker");
    title.setCharacterSize(18);
    title.setFillColor(sf::Color(204,204,204));
    title.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 10);
    for (int i=0;i<4;++i) {
        rgbText[i].setCharacterSize(12);
        rgbText[i].setFillColor(sf::Color(204,204,204));
        rgbBars[i].setSize({120,12});
        rgbBars[i].setFillColor(sf::Color(51,51,55));
        rgbBars[i].setOutlineColor(sf::Color(63,63,70));
        rgbBars[i].setOutlineThickness(1);
    }
    for (int i=0;i<4;++i) {
        float y = panel.getPosition().y + 70 + i*40;
        float x = panel.getPosition().x + 400;
        rgbBars[i].setPosition(x, y);
        rgbText[i].setPosition(x, y-16);
    }
    rgbText[0].setString("R");
    rgbText[1].setString("G");
    rgbText[2].setString("B");
    rgbText[3].setString("A");
}

void ColorPickerDialog::show(bool editForeground) {
    editingForeground = editForeground;
    visible = true;
    sf::Color base = editForeground ? ColorManager::instance().getForeground() : ColorManager::instance().getBackground();
    rgba[0]=base.r; rgba[1]=base.g; rgba[2]=base.b; rgba[3]=base.a;
    rgbToHsv(base, hue, sat, val);
}

void ColorPickerDialog::hide() { visible = false; }

sf::Color ColorPickerDialog::getCurrentColor() const {
    return sf::Color(rgba[0], rgba[1], rgba[2], rgba[3]);
}

void ColorPickerDialog::setCurrentColor(const sf::Color& c) {
    rgba[0]=c.r; rgba[1]=c.g; rgba[2]=c.b; rgba[3]=c.a;
    rgbToHsv(c, hue, sat, val);
}

void ColorPickerDialog::updateFromHSV() {
    sf::Color c = hsvToRgb(hue, sat, val);
    rgba[0]=c.r; rgba[1]=c.g; rgba[2]=c.b;
}

void ColorPickerDialog::updateFromRGBA() {
    sf::Color c(rgba[0], rgba[1], rgba[2]);
    float h,s,v;
    rgbToHsv(c, h, s, v);
    hue=h; sat=s; val=v;
}

void ColorPickerDialog::applyColor() {
    sf::Color c = getCurrentColor();
    if (editingForeground) ColorManager::instance().setForeground(c);
    else ColorManager::instance().setBackground(c);
}

void ColorPickerDialog::drawHistory(sf::RenderWindow& window) {
    historyRects.clear();
    const auto& hist = ColorManager::instance().getHistory();
    float startX = panel.getPosition().x + 20;
    float y = panel.getPosition().y + 290;
    for (size_t i=0;i<hist.size() && i<12;i++) {
        sf::RectangleShape r;
        r.setSize({24,24});
        r.setFillColor(hist[i]);
        r.setOutlineColor(sf::Color(63,63,70));
        r.setOutlineThickness(1);
        r.setPosition(startX + i*26, y);
        historyRects.push_back(r);
    }
    for (auto& r : historyRects) window.draw(r);
}

bool ColorPickerDialog::handleHistoryClick(const sf::Vector2f& pos) {
    for (auto& r : historyRects) {
        if (r.getGlobalBounds().contains(pos)) {
            setCurrentColor(r.getFillColor());
            applyColor();
            return true;
        }
    }
    return false;
}

void ColorPickerDialog::draw(sf::RenderWindow& window) {
    if (!visible) return;
    window.draw(backdrop);
    window.draw(panel);
    if (fontLoaded) window.draw(title);
    sf::RenderTexture hueTex; hueTex.create(static_cast<unsigned int>(hueBar.getSize().x), static_cast<unsigned int>(hueBar.getSize().y));
    for (int x=0;x<static_cast<int>(hueBar.getSize().x);++x) {
        float h = 360.f * x / hueBar.getSize().x;
        sf::RectangleShape px; px.setSize({1, hueBar.getSize().y}); px.setPosition(hueBar.getPosition().x + x, hueBar.getPosition().y);
        px.setFillColor(hsvToRgb(h,1,1));
        window.draw(px);
    }
    window.draw(svBox);
    for (int y=0; y<static_cast<int>(svBox.getSize().y); ++y) {
        for (int x=0; x<static_cast<int>(svBox.getSize().x); ++x) {
            float s = static_cast<float>(x) / svBox.getSize().x;
            float v = 1.f - static_cast<float>(y) / svBox.getSize().y;
            sf::Color c = hsvToRgb(hue, s, v);
            sf::RectangleShape px; px.setSize({1,1}); px.setPosition(svBox.getPosition().x + x, svBox.getPosition().y + y);
            px.setFillColor(c);
            window.draw(px);
        }
    }
    for (int i=0;i<4;++i) window.draw(rgbBars[i]);
    if (fontLoaded) {
        for (int i=0;i<4;++i) window.draw(rgbText[i]);
    }
    window.draw(okButton);
    if (fontLoaded) {
        sf::FloatRect b = okText.getLocalBounds();
        okText.setPosition(okButton.getPosition().x + (okButton.getSize().x - b.width)/2, okButton.getPosition().y + (okButton.getSize().y - b.height)/2 - 4);
        window.draw(okText);
    }
    sf::RectangleShape preview; preview.setSize({60,60}); preview.setPosition(panel.getPosition().x + 420, panel.getPosition().y + 220);
    preview.setFillColor(getCurrentColor());
    window.draw(preview);
    drawHistory(window);
}

void ColorPickerDialog::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (!visible) return;
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i realPixelPos = sf::Mouse::getPosition(window);
        // Apply vertical offset to compensate for window titlebar
        sf::Vector2f p(static_cast<float>(realPixelPos.x), static_cast<float>(realPixelPos.y - 30));
        if (okButton.getGlobalBounds().contains(p)) { applyColor(); hide(); return; }
        if (handleHistoryClick(p)) return;
        if (hueBar.getGlobalBounds().contains(p)) {
            draggingHue = true;
            float x = p.x - hueBar.getPosition().x;
            x = std::max(0.f, std::min(x, hueBar.getSize().x));
            hue = 360.f * x / hueBar.getSize().x;
            updateFromHSV();
            return;
        }
        if (svBox.getGlobalBounds().contains(p)) {
            draggingSV = true;
            float x = p.x - svBox.getPosition().x;
            float y = p.y - svBox.getPosition().y;
            sat = std::max(0.f, std::min(1.f, x / svBox.getSize().x));
            val = std::max(0.f, std::min(1.f, 1.f - y / svBox.getSize().y));
            updateFromHSV();
            return;
        }
        for (int i=0;i<4;++i) {
            if (rgbBars[i].getGlobalBounds().contains(p)) { draggingRGBA = i; return; }
        }
    }
    if (event.type == sf::Event::MouseButtonReleased) {
        draggingHue = false; draggingSV = false; draggingRGBA = -1;
    }
    if (event.type == sf::Event::MouseMoved) {
        // Use real mouse position from window for dragging
        sf::Vector2i realPixelPos = sf::Mouse::getPosition(window);
        // Apply vertical offset to compensate for window titlebar
        sf::Vector2f p(static_cast<float>(realPixelPos.x), static_cast<float>(realPixelPos.y - 30));
        if (draggingHue) {
            float x = p.x - hueBar.getPosition().x;
            x = std::max(0.f, std::min(x, hueBar.getSize().x));
            hue = 360.f * x / hueBar.getSize().x;
            updateFromHSV();
        }
        if (draggingSV) {
            float x = p.x - svBox.getPosition().x;
            float y = p.y - svBox.getPosition().y;
            sat = std::max(0.f, std::min(1.f, x / svBox.getSize().x));
            val = std::max(0.f, std::min(1.f, 1.f - y / svBox.getSize().y));
            updateFromHSV();
        }
        if (draggingRGBA >= 0) {
            float x = p.x - rgbBars[draggingRGBA].getPosition().x;
            x = std::max(0.f, std::min(x, rgbBars[draggingRGBA].getSize().x));
            int value = static_cast<int>(255.f * x / rgbBars[draggingRGBA].getSize().x);
            rgba[draggingRGBA] = std::max(0, std::min(255, value));
            updateFromRGBA();
        }
    }
}
