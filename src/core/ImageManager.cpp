#include "ImageManager.hpp"
#include <iostream>
#include <algorithm>

ImageManager::ImageManager() {
    tabManager.setOnTabSelected([this](size_t imageIndex) {
        onTabSelected(imageIndex);
    });
    
    tabManager.setOnTabCloseRequested([this](size_t tabIndex) {
        onTabCloseRequested(tabIndex);
    });
    
    saveConfirmDialog.setOnSave([this]() {
        if (pendingCloseIndex < images.size() && onSaveRequest) {
            onSaveRequest(pendingCloseIndex);
        }
    });
    
    saveConfirmDialog.setOnDontSave([this]() {
        if (pendingCloseIndex < images.size()) {
            forceCloseImage(pendingCloseIndex);
        }
        pendingCloseIndex = SIZE_MAX;
    });
    
    saveConfirmDialog.setOnCancel([this]() {
        pendingCloseIndex = SIZE_MAX;
    });
}

void ImageManager::createNewImage(int width, int height, const std::string& name) {
    auto newImage = std::make_unique<Image>(width, height, name);
    images.push_back(std::move(newImage));
    currentImageIndex = images.size() - 1;
    
    tabManager.addTab(name, currentImageIndex);
    
    fitCurrentImageToView();
    std::cout << "Nouvelle image créée: " << name << " (" << width << "x" << height << ")" << std::endl;
}

void ImageManager::openImage(const std::string& filepath) {
    auto newImage = std::make_unique<Image>(filepath);
    std::string imageName = filepath.substr(filepath.find_last_of("/\\") + 1);
    newImage->setFilePath(filepath);
    
    images.push_back(std::move(newImage));
    currentImageIndex = images.size() - 1;
    
    tabManager.addTab(imageName, currentImageIndex);
    
    fitCurrentImageToView();
}

void ImageManager::closeImage(size_t index) {
    if (index >= images.size()) return;
    
    tabManager.removeTab(index);
    
    images.erase(images.begin() + index);
    if (images.empty()) {
        currentImageIndex = 0;
    } else if (currentImageIndex >= images.size()) {
        currentImageIndex = images.size() - 1;
    }
    updateImagePositions();
}

void ImageManager::requestCloseImage(size_t index) {
    if (index >= images.size()) return;
    
    if (images[index]->getModified()) {
        pendingCloseIndex = index;
        saveConfirmDialog.show(images[index]->getName());
    } else {
        forceCloseImage(index);
    }
}

void ImageManager::forceCloseImage(size_t index) {
    closeImage(index);
}

void ImageManager::closeCurrentImage() {
    if (!images.empty()) {
        requestCloseImage(currentImageIndex);
    }
}

void ImageManager::setActiveImage(size_t index) {
    if (index < images.size()) {
        currentImageIndex = index;
        tabManager.setActiveTab(index);
        centerCurrentImage();
    }
}

void ImageManager::nextImage() {
    if (!images.empty()) {
        currentImageIndex = (currentImageIndex + 1) % images.size();
        centerCurrentImage();
    }
}

void ImageManager::previousImage() {
    if (!images.empty()) {
        if (currentImageIndex == 0) {
            currentImageIndex = images.size() - 1;
        } else {
            currentImageIndex--;
        }
        centerCurrentImage();
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

void ImageManager::zoomInAt(const sf::Vector2f& centerPoint) {
    Image* current = getCurrentImage();
    if (current) {
        current->zoomInAt(centerPoint);
    }
}

void ImageManager::zoomOutAt(const sf::Vector2f& centerPoint) {
    Image* current = getCurrentImage();
    if (current) {
        current->zoomOutAt(centerPoint);
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
        current->draw(window, current->getViewPosition());
    }
}

void ImageManager::centerCurrentImage() {
    Image* current = getCurrentImage();
    if (current) {
        sf::Vector2i imageSize = current->getOriginalSize();
        float zoom = current->getZoom();
        
        // Calculer la position pour centrer l'image dans la zone de travail
        float centerX = imageViewPosition.x + (viewportSize.x - imageSize.x * zoom) / 2.0f;
        float centerY = imageViewPosition.y + (viewportSize.y - imageSize.y * zoom) / 2.0f;
        
        current->setViewPosition({centerX, centerY});
    }
}

void ImageManager::fitCurrentImageToView() {
    Image* current = getCurrentImage();
    if (current) {
        sf::Vector2i imageSize = current->getOriginalSize();
        
        // Calculer le zoom pour que l'image tienne dans la zone de travail
        float scaleX = viewportSize.x / imageSize.x;
        float scaleY = viewportSize.y / imageSize.y;
        float optimalZoom = std::min(scaleX, scaleY);
        
        // Ajuster le zoom selon la taille de l'image
        if (imageSize.x > 3000 || imageSize.y > 3000) {
            // Pour les très grandes images, utiliser 80% pour avoir plus de marge
            optimalZoom *= 0.8f;
        } else if (imageSize.x < 800 && imageSize.y < 600) {
            // Pour les petites images, utiliser 90% pour bien les voir
            optimalZoom *= 0.9f;
        } else {
            // Pour les images moyennes, utiliser 85%
            optimalZoom *= 0.85f;
        }
        
        // S'assurer que le zoom n'est ni trop petit ni trop grand
        optimalZoom = std::max(0.1f, std::min(2.0f, optimalZoom));
        
        current->setZoom(optimalZoom);
        centerCurrentImage();
    }
}

void ImageManager::updateImagePositions() {
    // Cette méthode n'est plus nécessaire car chaque image garde sa position individuelle
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

void ImageManager::drawTabs(sf::RenderWindow& window) {
    tabManager.draw(window);
}

void ImageManager::drawSaveDialog(sf::RenderWindow& window) {
    saveConfirmDialog.draw(window);
}

bool ImageManager::handleTabClick(const sf::Vector2f& mousePos) {
    return tabManager.handleClick(mousePos);
}

void ImageManager::handleTabMouseMove(const sf::Vector2f& mousePos) {
    tabManager.handleMouseMove(mousePos);
}

void ImageManager::handleSaveDialogEvent(const sf::Event& event) {
    saveConfirmDialog.handleEvent(event);
}

void ImageManager::notifyImageModified(size_t imageIndex) {
    if (imageIndex < images.size()) {
        tabManager.setTabModified(imageIndex, images[imageIndex]->getModified());
    }
}

void ImageManager::onTabSelected(size_t imageIndex) {
    for (size_t i = 0; i < images.size(); ++i) {
        if (i == imageIndex) {
            currentImageIndex = i;
            centerCurrentImage();
            break;
        }
    }
}

void ImageManager::onTabCloseRequested(size_t tabIndex) {
    if (tabIndex < images.size()) {
        requestCloseImage(tabIndex);
    }
}
