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

FlipCommand::FlipCommand(Image* image, FlipType type) : targetImage(image), flipType(type) {
    saveBackup();
}

void FlipCommand::execute() {
    if (!targetImage) return;
    
    sf::Image imageData = targetImage->getImageData();
    sf::Vector2u size = imageData.getSize();
    sf::Image flippedImage;
    flippedImage.create(size.x, size.y, sf::Color::Transparent);
    
    for (unsigned int y = 0; y < size.y; ++y) {
        for (unsigned int x = 0; x < size.x; ++x) {
            sf::Color pixel = imageData.getPixel(x, y);
            if (flipType == HORIZONTAL) {
                flippedImage.setPixel(size.x - 1 - x, y, pixel);
            } else {
                flippedImage.setPixel(x, size.y - 1 - y, pixel);
            }
        }
    }
    
    targetImage->setImageContent(flippedImage);
    targetImage->markAsModified();
}

void FlipCommand::undo() {
    restoreBackup();
}

std::unique_ptr<Command> FlipCommand::clone() const {
    return std::make_unique<FlipCommand>(targetImage, flipType);
}

void FlipCommand::saveBackup() {
    if (targetImage) {
        backupImage = targetImage->getImageData();
    }
}

void FlipCommand::restoreBackup() {
    if (!targetImage) return;
    
    sf::Texture backupTexture;
    backupTexture.loadFromImage(backupImage);
    sf::Sprite backupSprite(backupTexture);
    
    auto& texture = targetImage->getTexture();
    texture.clear(sf::Color::Transparent);
    texture.draw(backupSprite);
    texture.display();
}

RotateCommand::RotateCommand(Image* image, RotationType type) : targetImage(image), rotationType(type) {
    saveBackup();
    if (image) {
        originalSize = image->getTexture().getSize();
    }
}

void RotateCommand::execute() {
    if (!targetImage) return;
    
    sf::Image imageData = targetImage->getImageData();
    sf::Vector2u size = imageData.getSize();
    sf::Image rotatedImage;
    
    if (rotationType == ROTATE_90 || rotationType == ROTATE_270) {
        rotatedImage.create(size.y, size.x, sf::Color::Transparent);
        
        for (unsigned int y = 0; y < size.y; ++y) {
            for (unsigned int x = 0; x < size.x; ++x) {
                sf::Color pixel = imageData.getPixel(x, y);
                if (rotationType == ROTATE_90) {
                    rotatedImage.setPixel(size.y - 1 - y, x, pixel);
                } else {
                    rotatedImage.setPixel(y, size.x - 1 - x, pixel);
                }
            }
        }
        
        targetImage->setImageContent(rotatedImage);
    } else {
        rotatedImage.create(size.x, size.y, sf::Color::Transparent);
        
        for (unsigned int y = 0; y < size.y; ++y) {
            for (unsigned int x = 0; x < size.x; ++x) {
                sf::Color pixel = imageData.getPixel(x, y);
                rotatedImage.setPixel(size.x - 1 - x, size.y - 1 - y, pixel);
            }
        }
        
        targetImage->setImageContent(rotatedImage);
    }
    
    targetImage->markAsModified();
}

void RotateCommand::undo() {
    if (targetImage) {
        targetImage->resize(originalSize.x, originalSize.y);
    }
    restoreBackup();
}

std::unique_ptr<Command> RotateCommand::clone() const {
    return std::make_unique<RotateCommand>(targetImage, rotationType);
}

void RotateCommand::saveBackup() {
    if (targetImage) {
        backupImage = targetImage->getImageData();
    }
}

void RotateCommand::restoreBackup() {
    if (!targetImage) return;
    
    sf::Texture backupTexture;
    backupTexture.loadFromImage(backupImage);
    sf::Sprite backupSprite(backupTexture);
    
    auto& texture = targetImage->getTexture();
    texture.clear(sf::Color::Transparent);
    texture.draw(backupSprite);
    texture.display();
}

CropCommand::CropCommand(Image* image, const sf::IntRect& cropRect) : targetImage(image), cropRegion(cropRect) {
    saveBackup();
    if (image) {
        originalSize = image->getTexture().getSize();
    }
}

void CropCommand::execute() {
    if (!targetImage) return;
    
    sf::Image imageData = targetImage->getImageData();
    sf::Image croppedImage;
    croppedImage.create(cropRegion.width, cropRegion.height, sf::Color::Transparent);
    
    for (int y = 0; y < cropRegion.height; ++y) {
        for (int x = 0; x < cropRegion.width; ++x) {
            int srcX = cropRegion.left + x;
            int srcY = cropRegion.top + y;
            
            if (srcX >= 0 && srcX < static_cast<int>(imageData.getSize().x) &&
                srcY >= 0 && srcY < static_cast<int>(imageData.getSize().y)) {
                sf::Color pixel = imageData.getPixel(srcX, srcY);
                croppedImage.setPixel(x, y, pixel);
            }
        }
    }
    
    targetImage->setImageContent(croppedImage);
    targetImage->markAsModified();
}

void CropCommand::undo() {
    if (targetImage) {
        targetImage->resize(originalSize.x, originalSize.y);
    }
    restoreBackup();
}

std::unique_ptr<Command> CropCommand::clone() const {
    return std::make_unique<CropCommand>(targetImage, cropRegion);
}

void CropCommand::saveBackup() {
    if (targetImage) {
        backupImage = targetImage->getImageData();
    }
}

void CropCommand::restoreBackup() {
    if (!targetImage) return;
    
    sf::Texture backupTexture;
    backupTexture.loadFromImage(backupImage);
    sf::Sprite backupSprite(backupTexture);
    
    auto& texture = targetImage->getTexture();
    texture.clear(sf::Color::Transparent);
    texture.draw(backupSprite);
    texture.display();
}
