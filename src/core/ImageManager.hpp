#pragma once
#include "Image.hpp"
#include "../ui/TabManager.hpp"
#include "../ui/SaveConfirmDialog.hpp"
#include <vector>
#include <memory>
#include <string>
#include <functional>

class ImageManager {
public:
    ImageManager();
    ~ImageManager() = default;
    
    void createNewImage(int width, int height, const std::string& name);
    void openImage(const std::string& filepath);
    void closeImage(size_t index);
    void closeCurrentImage();
    void requestCloseImage(size_t index);
    void forceCloseImage(size_t index);
    
    void setActiveImage(size_t index);
    void nextImage();
    void previousImage();
    
    Image* getCurrentImage();
    Image* getImage(size_t index);
    const Image* getCurrentImage() const;
    const Image* getImage(size_t index) const;
    size_t getImageCount() const { return images.size(); }
    size_t getCurrentImageIndex() const { return currentImageIndex; }
    
    void zoomIn();
    void zoomOut();
    void zoomInAt(const sf::Vector2f& centerPoint);
    void zoomOutAt(const sf::Vector2f& centerPoint);
    void resetZoom();
    void setZoom(float zoom);
    
    void draw(sf::RenderWindow& window);
    void drawTabs(sf::RenderWindow& window);
    void drawSaveDialog(sf::RenderWindow& window);
    
    void setViewportSize(const sf::Vector2f& size) { viewportSize = size; }
    void centerCurrentImage();
    void fitCurrentImageToView();
    
    sf::Vector2f worldToImage(const sf::Vector2f& worldPos) const;
    sf::Vector2f imageToWorld(const sf::Vector2f& imagePos) const;
    
    bool isPositionInCurrentImage(const sf::Vector2f& worldPos) const;
    
    bool handleTabClick(const sf::Vector2f& mousePos);
    void handleTabMouseMove(const sf::Vector2f& mousePos);
    void handleSaveDialogEvent(const sf::Event& event);
    
    void setOnSaveRequest(std::function<void(size_t)> callback) { onSaveRequest = callback; }
    void setOnSaveAsRequest(std::function<void(size_t)> callback) { onSaveAsRequest = callback; }
    
    void notifyImageModified(size_t imageIndex);
    
private:
    std::vector<std::unique_ptr<Image>> images;
    size_t currentImageIndex = 0;
    sf::Vector2f viewportSize = {1720, 1050};
    sf::Vector2f imageViewPosition = {220, 82};
    
    TabManager tabManager;
    SaveConfirmDialog saveConfirmDialog;
    size_t pendingCloseIndex = SIZE_MAX;
    
    std::function<void(size_t)> onSaveRequest;
    std::function<void(size_t)> onSaveAsRequest;
    
    void updateImagePositions();
    void onTabSelected(size_t imageIndex);
    void onTabCloseRequested(size_t tabIndex);
};
