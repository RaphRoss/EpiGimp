#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

enum class SelectionMode {
    Replace,
    Add,
    Subtract,
    Intersect
};

class Selection {
public:
    Selection();
    
    void setRectangle(const sf::FloatRect& rect, SelectionMode mode = SelectionMode::Replace);
    void addRectangle(const sf::FloatRect& rect);
    void subtractRectangle(const sf::FloatRect& rect);
    void intersectRectangle(const sf::FloatRect& rect);
    
    void clear();
    bool isEmpty() const;
    bool contains(const sf::Vector2f& point) const;
    
    sf::FloatRect getBounds() const;
    const std::vector<sf::FloatRect>& getRegions() const { return regions; }
    
    void draw(sf::RenderWindow& window, const sf::Vector2f& offset, float zoom) const;
    
    void move(const sf::Vector2f& delta);
    void scale(float factor);

private:
    std::vector<sf::FloatRect> regions;
    mutable std::vector<sf::Vertex> marchingAnts;
    mutable float antOffset = 0.0f;
    
    void updateMarchingAnts(const sf::Vector2f& offset, float zoom) const;
    void optimizeRegions();
    bool rectsIntersect(const sf::FloatRect& a, const sf::FloatRect& b) const;
    sf::FloatRect rectIntersection(const sf::FloatRect& a, const sf::FloatRect& b) const;
};
