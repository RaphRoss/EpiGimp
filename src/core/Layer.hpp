#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

enum class BlendMode {
    Normal,
    Multiply,
    Screen,
    Overlay,
    Addition,
    Subtract,
    Difference,
    Lighten,
    Darken
};

class Layer {
public:
    Layer(unsigned int width, unsigned int height, const std::string& name = "Layer");
    Layer(const sf::Image& imageData, const std::string& name = "Layer");
    ~Layer() = default;

    // Getters
    const std::string& getName() const { return name; }
    bool isVisible() const { return visible; }
    float getOpacity() const { return opacity; }
    BlendMode getBlendMode() const { return blendMode; }
    sf::RenderTexture& getTexture() { return renderTexture; }
    const sf::RenderTexture& getTexture() const { return renderTexture; }
    sf::Image getImageData() const;
    sf::Sprite getSprite() const;
    sf::Vector2u getSize() const { return sf::Vector2u(width, height); }
    
    // Thumbnail for UI
    sf::Image getThumbnail(unsigned int maxWidth, unsigned int maxHeight) const;

    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setVisible(bool vis) { visible = vis; }
    void setOpacity(float op) { opacity = std::max(0.f, std::min(1.f, op)); }
    void setBlendMode(BlendMode mode) { blendMode = mode; }
    void setImageData(const sf::Image& imageData);

    // Drawing
    void clear(const sf::Color& color = sf::Color::Transparent);
    void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
    void display();

private:
    std::string name;
    sf::RenderTexture renderTexture;
    unsigned int width;
    unsigned int height;
    bool visible;
    float opacity;
    BlendMode blendMode;
};
