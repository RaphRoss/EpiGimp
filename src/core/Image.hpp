#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Image {
public:
    Image(int width, int height, const std::string& name = "Untitled");
    Image(const std::string& filepath);
    ~Image() = default;
    void draw(sf::RenderWindow& window, const sf::Vector2f& position);
    void setZoom(float zoom);
    void zoomIn();
    void zoomOut();
    void resetZoom();
    sf::RenderTexture& getTexture();
    sf::Vector2i getOriginalSize() const { return originalSize; }
    sf::Vector2f getDisplaySize() const;
    float getZoom() const { return zoomLevel; }
    std::string getName() const { return imageName; }
    void setName(const std::string& name) { imageName = name; }
    void saveToFile(const std::string& filename);
    void setViewPosition(const sf::Vector2f& pos) { viewPosition = pos; }
    sf::Vector2f getViewPosition() const { return viewPosition; }
    sf::FloatRect getBounds() const;
    sf::Vector2f worldToImage(const sf::Vector2f& worldPos) const;
    sf::Vector2f imageToWorld(const sf::Vector2f& imagePos) const;

private:
    sf::RenderTexture renderTexture;
    sf::Sprite sprite;
    sf::Vector2i originalSize;
    float zoomLevel = 1.0f;
    std::string imageName;
    sf::Vector2f viewPosition = {220, 50};
    void updateSprite();
    void setupTexture();
};
