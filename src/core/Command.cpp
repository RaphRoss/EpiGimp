#include "Command.hpp"
#include "Image.hpp"
#include <algorithm>
#include <cmath>

DrawCommand::DrawCommand(Image* image, const sf::Vector2f& from, const sf::Vector2f& to, const sf::Color& color)
    : targetImage(image), fromPos(from), toPos(to), drawColor(color) {
    saveBackup();
}

void DrawCommand::execute() {
    if (!targetImage) return;
    
    sf::Vertex line[] = {
        sf::Vertex(fromPos, drawColor),
        sf::Vertex(toPos, drawColor)
    };
    
    auto& texture = targetImage->getTexture();
    texture.draw(line, 2, sf::Lines);
    texture.display();
}

void DrawCommand::undo() {
    restoreBackup();
}

std::unique_ptr<Command> DrawCommand::clone() const {
    return std::make_unique<DrawCommand>(targetImage, fromPos, toPos, drawColor);
}

void DrawCommand::saveBackup() {
    if (!targetImage) return;
    
    float minX = std::min(fromPos.x, toPos.x) - 2;
    float minY = std::min(fromPos.y, toPos.y) - 2;
    float maxX = std::max(fromPos.x, toPos.x) + 2;
    float maxY = std::max(fromPos.y, toPos.y) + 2;
    
    regionPos = sf::Vector2i(static_cast<int>(minX), static_cast<int>(minY));
    regionSize = sf::Vector2i(static_cast<int>(maxX - minX), static_cast<int>(maxY - minY));
    
    sf::Vector2i imageSize = targetImage->getOriginalSize();
    regionPos.x = std::max(0, std::min(regionPos.x, imageSize.x - 1));
    regionPos.y = std::max(0, std::min(regionPos.y, imageSize.y - 1));
    regionSize.x = std::max(1, std::min(regionSize.x, imageSize.x - regionPos.x));
    regionSize.y = std::max(1, std::min(regionSize.y, imageSize.y - regionPos.y));
    
    sf::Texture tex = targetImage->getTexture().getTexture();
    sf::Image fullImage = tex.copyToImage();
    
    backupRegion.create(regionSize.x, regionSize.y);
    backupRegion.copy(fullImage, 0, 0, sf::IntRect(regionPos.x, regionPos.y, regionSize.x, regionSize.y));
}

void DrawCommand::restoreBackup() {
    if (!targetImage) return;
    
    sf::Texture backupTexture;
    backupTexture.loadFromImage(backupRegion);
    sf::Sprite backupSprite(backupTexture);
    backupSprite.setPosition(static_cast<float>(regionPos.x), static_cast<float>(regionPos.y));
    
    auto& texture = targetImage->getTexture();
    texture.draw(backupSprite);
    texture.display();
}

StrokeCommand::StrokeCommand(Image* image) : targetImage(image) {
}

void StrokeCommand::addPoint(const sf::Vector2f& point, const sf::Color& color) {
    if (!backupSaved) {
        saveBackup();
        backupSaved = true;
    }
    points.push_back({point, color});
}

void StrokeCommand::execute() {
    if (!targetImage || points.empty()) return;
    
    auto& texture = targetImage->getTexture();
    sf::RenderStates states;
    states.blendMode = sf::BlendAlpha;
    
    for (const auto& point : points) {
        sf::CircleShape brush(2.5f);
        brush.setFillColor(point.second);
        brush.setPosition(point.first.x - 2.5f, point.first.y - 2.5f);
        texture.draw(brush, states);
    }
    
    for (size_t i = 1; i < points.size(); ++i) {
        sf::Vector2f start = points[i-1].first;
        sf::Vector2f end = points[i].first;
        sf::Color color = points[i].second;
        
        float dx = end.x - start.x;
        float dy = end.y - start.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        
        if (distance > 1.0f) {
            int steps = static_cast<int>(distance);
            for (int j = 0; j <= steps; ++j) {
                float t = static_cast<float>(j) / steps;
                sf::Vector2f point(start.x + t * dx, start.y + t * dy);
                
                sf::CircleShape brush(2.5f);
                brush.setFillColor(color);
                brush.setPosition(point.x - 2.5f, point.y - 2.5f);
                texture.draw(brush, states);
            }
        }
    }
    texture.display();
}

void StrokeCommand::undo() {
    restoreBackup();
}

std::unique_ptr<Command> StrokeCommand::clone() const {
    auto clone = std::make_unique<StrokeCommand>(targetImage);
    clone->points = points;
    clone->backupImage = backupImage;
    clone->backupSaved = backupSaved;
    return std::move(clone);
}

void StrokeCommand::saveBackup() {
    if (!targetImage) return;
    
    sf::Texture tex = targetImage->getTexture().getTexture();
    backupImage = tex.copyToImage();
}

void StrokeCommand::restoreBackup() {
    if (!targetImage) return;
    
    sf::Texture backupTexture;
    backupTexture.loadFromImage(backupImage);
    sf::Sprite backupSprite(backupTexture);
    
    auto& texture = targetImage->getTexture();
    texture.clear(sf::Color::Transparent);
    texture.draw(backupSprite);
    texture.display();
}
