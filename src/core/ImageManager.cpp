#include "ImageManager.hpp"
#include <iostream>

ImageManager::ImageManager() {
}

void ImageManager::createNewImage(int width, int height, const std::string& name) {
    auto newImage = std::make_unique<Image>(width, height, name);
    newImage->setViewPosition(imageViewPosition);
    images.push_back(std::move(newImage));
    currentImageIndex = images.size() - 1;
    updateImagePositions();
    std::cout << "Nouvelle image créée: " << name << " (" << width << "x" << height << ")" << std::endl;
}

void ImageManager::openImage(const std::string& filepath) {
    auto newImage = std::make_unique<Image>(filepath);
    newImage->setViewPosition(imageViewPosition);
    images.push_back(std::move(newImage));
    currentImageIndex = images.size() - 1;
    updateImagePositions();
}

void ImageManager::closeImage(size_t index) {
    if (index >= images.size()) return;
    images.erase(images.begin() + index);
    if (images.empty()) {
        currentImageIndex = 0;
    } else if (currentImageIndex >= images.size()) {
        currentImageIndex = images.size() - 1;
    }
    updateImagePositions();
}

void ImageManager::closeCurrentImage() {
    if (!images.empty()) {
        closeImage(currentImageIndex);
    }
}

void ImageManager::setActiveImage(size_t index) {
    if (index < images.size()) {
        currentImageIndex = index;
        updateImagePositions();
    }
}

void ImageManager::nextImage() {
    if (!images.empty()) {
        currentImageIndex = (currentImageIndex + 1) % images.size();
        updateImagePositions();
    }
}

void ImageManager::previousImage() {
    if (!images.empty()) {
        if (currentImageIndex == 0) {
            currentImageIndex = images.size() - 1;
        } else {
            currentImageIndex--;
        }
        updateImagePositions();
    }
}

Image* ImageManager::getCurrentImage() {
    if (images.empty() || currentImageIndex >= images.size()) {
        return nullptr;
    }
    return images[currentImageIndex].get();
}

Image* ImageManager::getImage(size_t index) {
    if (index >= images.size()) {
        return nullptr;
    }
    return images[index].get();
}

const Image* ImageManager::getCurrentImage() const {
    if (images.empty() || currentImageIndex >= images.size()) {
        return nullptr;
    }
    return images[currentImageIndex].get();
}

const Image* ImageManager::getImage(size_t index) const {
    if (index >= images.size()) {
        return nullptr;
    }
    return images[index].get();
}

void ImageManager::zoomIn() {
    Image* current = getCurrentImage();
    if (current) {
        current->zoomIn();
    }
}

void ImageManager::zoomOut() {
    Image* current = getCurrentImage();
    if (current) {
        current->zoomOut();
    }
}

void ImageManager::resetZoom() {
    Image* current = getCurrentImage();
    if (current) {
        current->resetZoom();
    }
}

void ImageManager::setZoom(float zoom) {
    Image* current = getCurrentImage();
    if (current) {
        current->setZoom(zoom);
    }
}

void ImageManager::draw(sf::RenderWindow& window) {
    Image* current = getCurrentImage();
    if (current) {
        current->draw(window, imageViewPosition);
    }
}

void ImageManager::updateImagePositions() {
    for (auto& image : images) {
        image->setViewPosition(imageViewPosition);
    }
}

sf::Vector2f ImageManager::worldToImage(const sf::Vector2f& worldPos) const {
    const Image* current = getCurrentImage();
    if (current) {
        return current->worldToImage(worldPos);
    }
    return worldPos;
}

sf::Vector2f ImageManager::imageToWorld(const sf::Vector2f& imagePos) const {
    const Image* current = getCurrentImage();
    if (current) {
        return current->imageToWorld(imagePos);
    }
    return imagePos;
}

bool ImageManager::isPositionInCurrentImage(const sf::Vector2f& worldPos) const {
    const Image* current = getCurrentImage();
    if (current) {
        return current->getBounds().contains(worldPos);
    }
    return false;
}
