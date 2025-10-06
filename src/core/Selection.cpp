#include "Selection.hpp"
#include <algorithm>
#include <cmath>

Selection::Selection() {
}

void Selection::setRectangle(const sf::FloatRect& rect, SelectionMode mode) {
    switch (mode) {
        case SelectionMode::Replace:
            clear();
            regions.push_back(rect);
            break;
        case SelectionMode::Add:
            addRectangle(rect);
            break;
        case SelectionMode::Subtract:
            subtractRectangle(rect);
            break;
        case SelectionMode::Intersect:
            intersectRectangle(rect);
            break;
    }
    optimizeRegions();
}

void Selection::addRectangle(const sf::FloatRect& rect) {
    regions.push_back(rect);
    optimizeRegions();
}

void Selection::subtractRectangle(const sf::FloatRect& rect) {
    std::vector<sf::FloatRect> newRegions;
    
    for (const auto& region : regions) {
        if (!rectsIntersect(region, rect)) {
            newRegions.push_back(region);
            continue;
        }
        
        sf::FloatRect intersection = rectIntersection(region, rect);
        
        if (intersection.left > region.left) {
            newRegions.push_back(sf::FloatRect(
                region.left, region.top,
                intersection.left - region.left, region.height
            ));
        }
        
        if (intersection.left + intersection.width < region.left + region.width) {
            newRegions.push_back(sf::FloatRect(
                intersection.left + intersection.width, region.top,
                region.left + region.width - (intersection.left + intersection.width), region.height
            ));
        }
        
        if (intersection.top > region.top) {
            newRegions.push_back(sf::FloatRect(
                intersection.left, region.top,
                intersection.width, intersection.top - region.top
            ));
        }
        
        if (intersection.top + intersection.height < region.top + region.height) {
            newRegions.push_back(sf::FloatRect(
                intersection.left, intersection.top + intersection.height,
                intersection.width, region.top + region.height - (intersection.top + intersection.height)
            ));
        }
    }
    
    regions = std::move(newRegions);
    optimizeRegions();
}

void Selection::intersectRectangle(const sf::FloatRect& rect) {
    std::vector<sf::FloatRect> newRegions;
    
    for (const auto& region : regions) {
        if (rectsIntersect(region, rect)) {
            newRegions.push_back(rectIntersection(region, rect));
        }
    }
    
    regions = std::move(newRegions);
    optimizeRegions();
}

void Selection::clear() {
    regions.clear();
    marchingAnts.clear();
}

bool Selection::isEmpty() const {
    return regions.empty();
}

bool Selection::contains(const sf::Vector2f& point) const {
    for (const auto& region : regions) {
        if (region.contains(point)) {
            return true;
        }
    }
    return false;
}

sf::FloatRect Selection::getBounds() const {
    if (regions.empty()) {
        return sf::FloatRect(0, 0, 0, 0);
    }
    
    float minX = regions[0].left;
    float minY = regions[0].top;
    float maxX = regions[0].left + regions[0].width;
    float maxY = regions[0].top + regions[0].height;
    
    for (const auto& region : regions) {
        minX = std::min(minX, region.left);
        minY = std::min(minY, region.top);
        maxX = std::max(maxX, region.left + region.width);
        maxY = std::max(maxY, region.top + region.height);
    }
    
    return sf::FloatRect(minX, minY, maxX - minX, maxY - minY);
}

void Selection::draw(sf::RenderWindow& window, const sf::Vector2f& offset, float zoom) const {
    if (regions.empty()) return;
    
    updateMarchingAnts(offset, zoom);
    
    if (!marchingAnts.empty()) {
        window.draw(&marchingAnts[0], marchingAnts.size(), sf::Lines);
    }
}

void Selection::move(const sf::Vector2f& delta) {
    for (auto& region : regions) {
        region.left += delta.x;
        region.top += delta.y;
    }
}

void Selection::scale(float factor) {
    for (auto& region : regions) {
        region.left *= factor;
        region.top *= factor;
        region.width *= factor;
        region.height *= factor;
    }
}

void Selection::updateMarchingAnts(const sf::Vector2f& offset, float zoom) const {
    marchingAnts.clear();
    antOffset += 0.1f;
    
    for (const auto& region : regions) {
        sf::FloatRect screenRect(
            region.left * zoom + offset.x,
            region.top * zoom + offset.y,
            region.width * zoom,
            region.height * zoom
        );
        
        sf::Color antColor1(255, 255, 255, 200);
        sf::Color antColor2(0, 0, 0, 200);
        
        float dashLength = 8.0f;
        float totalPerimeter = 2 * (screenRect.width + screenRect.height);
        int numDashes = static_cast<int>(totalPerimeter / dashLength);
        
        for (int i = 0; i < numDashes; ++i) {
            float t = static_cast<float>(i) / numDashes;
            sf::Color color = ((i + static_cast<int>(antOffset * 2)) % 2 == 0) ? antColor1 : antColor2;
            
            sf::Vector2f start, end;
            
            if (t < 0.25f) {
                float localT = t * 4;
                start = sf::Vector2f(screenRect.left + localT * screenRect.width, screenRect.top);
                end = sf::Vector2f(screenRect.left + (localT + 0.05f) * screenRect.width, screenRect.top);
            } else if (t < 0.5f) {
                float localT = (t - 0.25f) * 4;
                start = sf::Vector2f(screenRect.left + screenRect.width, screenRect.top + localT * screenRect.height);
                end = sf::Vector2f(screenRect.left + screenRect.width, screenRect.top + (localT + 0.05f) * screenRect.height);
            } else if (t < 0.75f) {
                float localT = (t - 0.5f) * 4;
                start = sf::Vector2f(screenRect.left + screenRect.width - localT * screenRect.width, screenRect.top + screenRect.height);
                end = sf::Vector2f(screenRect.left + screenRect.width - (localT + 0.05f) * screenRect.width, screenRect.top + screenRect.height);
            } else {
                float localT = (t - 0.75f) * 4;
                start = sf::Vector2f(screenRect.left, screenRect.top + screenRect.height - localT * screenRect.height);
                end = sf::Vector2f(screenRect.left, screenRect.top + screenRect.height - (localT + 0.05f) * screenRect.height);
            }
            
            marchingAnts.push_back(sf::Vertex(start, color));
            marchingAnts.push_back(sf::Vertex(end, color));
        }
    }
}

void Selection::optimizeRegions() {
}

bool Selection::rectsIntersect(const sf::FloatRect& a, const sf::FloatRect& b) const {
    return !(a.left + a.width <= b.left || b.left + b.width <= a.left ||
             a.top + a.height <= b.top || b.top + b.height <= a.top);
}

sf::FloatRect Selection::rectIntersection(const sf::FloatRect& a, const sf::FloatRect& b) const {
    float left = std::max(a.left, b.left);
    float top = std::max(a.top, b.top);
    float right = std::min(a.left + a.width, b.left + b.width);
    float bottom = std::min(a.top + a.height, b.top + b.height);
    
    if (left < right && top < bottom) {
        return sf::FloatRect(left, top, right - left, bottom - top);
    }
    
    return sf::FloatRect(0, 0, 0, 0);
}
