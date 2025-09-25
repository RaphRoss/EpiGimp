#pragma once
#include "Image.hpp"
#include <vector>
#include <memory>
#include <string>

class ImageManager {
public:
    ImageManager();
    ~ImageManager() = default;
    
    // Gestion des images
    void createNewImage(int width, int height, const std::string& name);
    void openImage(const std::string& filepath);
    void closeImage(size_t index);
    void closeCurrentImage();
    
    // Navigation entre les images
    void setActiveImage(size_t index);
    void nextImage();
    void previousImage();
    
    // Accès aux images
    Image* getCurrentImage();
    Image* getImage(size_t index);
    const Image* getCurrentImage() const;
    const Image* getImage(size_t index) const;
    size_t getImageCount() const { return images.size(); }
    size_t getCurrentImageIndex() const { return currentImageIndex; }
    
    // Gestion du zoom pour l'image courante
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void setZoom(float zoom);
    
    // Gestion de l'affichage
    void draw(sf::RenderWindow& window);
    void setViewportSize(const sf::Vector2f& size) { viewportSize = size; }
    
    // Conversion de coordonnées pour l'image courante
    sf::Vector2f worldToImage(const sf::Vector2f& worldPos) const;
    sf::Vector2f imageToWorld(const sf::Vector2f& imagePos) const;
    
    // Vérifier si une position est dans l'image courante
    bool isPositionInCurrentImage(const sf::Vector2f& worldPos) const;
    
private:
    std::vector<std::unique_ptr<Image>> images;
    size_t currentImageIndex = 0;
    sf::Vector2f viewportSize = {1720, 1050}; // Taille par défaut (écran - UI)
    sf::Vector2f imageViewPosition = {220, 50}; // Position de départ des images
    
    void updateImagePositions();
};
