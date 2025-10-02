#include "Rulers.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

const float Rulers::RULER_SIZE = 25.0f;
sf::Font Rulers::font;
bool Rulers::fontLoaded = false;

Rulers::Rulers() {
    if (!fontLoaded) {
        if (!font.loadFromFile("src/assets/fonts/DejaVuSans-Bold.ttf")) {
            std::cerr << "Erreur: Impossible de charger la police pour les règles !" << std::endl;
            fontLoaded = false;
        } else {
            fontLoaded = true;
        }
    }
}

void Rulers::draw(sf::RenderWindow& window, const sf::Vector2f& imagePos, const sf::Vector2f& imageSize, 
                  float zoom, const sf::Vector2f& mousePos) {
    if (!isVisible) return;
    
    drawHorizontalRuler(window, imagePos, imageSize, zoom, mousePos);
    drawVerticalRuler(window, imagePos, imageSize, zoom, mousePos);
    drawCrosshair(window, mousePos, imagePos, imageSize);
}

float Rulers::calculateOptimalStep(float zoom) const {
    // Étapes possibles en pixels (dans l'image originale)
    const float baseSteps[] = {1, 2, 5, 10, 20, 25, 50, 100, 200, 250, 500, 1000, 2000, 2500, 5000, 10000};
    const int numSteps = sizeof(baseSteps) / sizeof(baseSteps[0]);
    
    // Taille minimale souhaitée à l'écran (en pixels)
    const float minScreenSize = 40.0f;
    const float maxScreenSize = 120.0f;
    
    // Trouver l'étape optimale
    for (int i = 0; i < numSteps; ++i) {
        float screenSize = baseSteps[i] * zoom;
        if (screenSize >= minScreenSize && screenSize <= maxScreenSize) {
            return baseSteps[i];
        }
        if (screenSize > maxScreenSize) {
            // Retourner l'étape précédente si elle existe
            return (i > 0) ? baseSteps[i - 1] : baseSteps[i];
        }
    }
    
    // Si aucune étape trouvée, utiliser la plus grande
    return baseSteps[numSteps - 1];
}

std::string Rulers::formatValue(float value, float step) const {
    std::stringstream ss;
    
    // Format intelligent basé sur la taille de l'étape
    if (step >= 1000) {
        // Affichage en K pour les grandes valeurs
        ss << std::fixed << std::setprecision(0) << (value / 1000.0f) << "K";
    } else if (step >= 100) {
        // Pas de décimales pour les centaines
        ss << std::fixed << std::setprecision(0) << value;
    } else if (step >= 10) {
        // Une décimale pour les dizaines si nécessaire
        if (std::fmod(value, 1.0f) == 0.0f) {
            ss << std::fixed << std::setprecision(0) << value;
        } else {
            ss << std::fixed << std::setprecision(1) << value;
        }
    } else {
        // Deux décimales pour les petites valeurs
        ss << std::fixed << std::setprecision(1) << value;
    }
    
    return ss.str();
}

void Rulers::drawHorizontalRuler(sf::RenderWindow& window, const sf::Vector2f& imagePos, 
                                const sf::Vector2f& imageSize, float zoom, const sf::Vector2f& mousePos) {
    // Background de la règle horizontale
    sf::RectangleShape rulerBg;
    rulerBg.setPosition(imagePos.x, imagePos.y - RULER_SIZE);
    rulerBg.setSize(sf::Vector2f(imageSize.x, RULER_SIZE));
    rulerBg.setFillColor(rulerColor);
    rulerBg.setOutlineColor(sf::Color(128, 128, 128));
    rulerBg.setOutlineThickness(1);
    window.draw(rulerBg);
    
    // Calculer l'étape optimale
    float step = calculateOptimalStep(zoom);
    float screenStep = step * zoom;
    
    // Calculer les bornes en coordonnées image
    float imageStartX = (imagePos.x < 0) ? -imagePos.x / zoom : 0;
    float imageEndX = (imagePos.x + imageSize.x > 1920) ? 
                      imageStartX + (1920 - imagePos.x) / zoom : 
                      imageStartX + imageSize.x / zoom;
    
    // Aligner le premier tick sur un multiple de step
    float firstTick = std::ceil(imageStartX / step) * step;
    
    for (float imageX = firstTick; imageX <= imageEndX; imageX += step) {
        float screenX = imagePos.x + imageX * zoom;
        
        // Vérifier si le tick est visible
        if (screenX < imagePos.x - 10 || screenX > imagePos.x + imageSize.x + 10) continue;
        
        // Ligne de graduation (plus longue tous les 5 ticks)
        bool isMajorTick = (std::fmod(imageX, step * 5) < 0.1);
        float lineHeight = isMajorTick ? RULER_SIZE - 5 : RULER_SIZE - 10;
        
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(screenX, imagePos.y - lineHeight), sf::Color::Black),
            sf::Vertex(sf::Vector2f(screenX, imagePos.y - 2), sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
        
        // Texte de graduation (seulement pour les ticks majeurs ou si assez d'espace)
        if (fontLoaded && (isMajorTick || screenStep > 60)) {
            sf::Text text;
            text.setFont(font);
            text.setString(formatValue(imageX, step));
            text.setCharacterSize(9);
            text.setFillColor(rulerTextColor);
            
            sf::FloatRect textBounds = text.getLocalBounds();
            text.setPosition(screenX + 2, imagePos.y - RULER_SIZE + 2);
            
            // Ne dessiner que si le texte ne sort pas de la règle
            if (screenX + textBounds.width + 2 <= imagePos.x + imageSize.x) {
                window.draw(text);
            }
        }
    }
    
    // Indicateur position souris
    if (mousePos.x >= imagePos.x && mousePos.x <= imagePos.x + imageSize.x) {
        sf::Vertex indicator[] = {
            sf::Vertex(sf::Vector2f(mousePos.x, imagePos.y - RULER_SIZE), crosshairColor),
            sf::Vertex(sf::Vector2f(mousePos.x, imagePos.y), crosshairColor)
        };
        window.draw(indicator, 2, sf::Lines);
    }
}

void Rulers::drawVerticalRuler(sf::RenderWindow& window, const sf::Vector2f& imagePos, 
                              const sf::Vector2f& imageSize, float zoom, const sf::Vector2f& mousePos) {
    // Background de la règle verticale
    sf::RectangleShape rulerBg;
    rulerBg.setPosition(imagePos.x - RULER_SIZE, imagePos.y);
    rulerBg.setSize(sf::Vector2f(RULER_SIZE, imageSize.y));
    rulerBg.setFillColor(rulerColor);
    rulerBg.setOutlineColor(sf::Color(128, 128, 128));
    rulerBg.setOutlineThickness(1);
    window.draw(rulerBg);
    
    // Calculer l'étape optimale
    float step = calculateOptimalStep(zoom);
    float screenStep = step * zoom;
    
    // Calculer les bornes en coordonnées image
    float imageStartY = (imagePos.y < 30) ? (30 - imagePos.y) / zoom : 0;
    float imageEndY = (imagePos.y + imageSize.y > 1055) ? 
                      imageStartY + (1055 - imagePos.y) / zoom : 
                      imageStartY + imageSize.y / zoom;
    
    // Aligner le premier tick sur un multiple de step
    float firstTick = std::ceil(imageStartY / step) * step;
    
    for (float imageY = firstTick; imageY <= imageEndY; imageY += step) {
        float screenY = imagePos.y + imageY * zoom;
        
        // Vérifier si le tick est visible
        if (screenY < imagePos.y - 10 || screenY > imagePos.y + imageSize.y + 10) continue;
        
        // Ligne de graduation (plus longue tous les 5 ticks)
        bool isMajorTick = (std::fmod(imageY, step * 5) < 0.1);
        float lineLength = isMajorTick ? RULER_SIZE - 5 : RULER_SIZE - 10;
        
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(imagePos.x - lineLength, screenY), sf::Color::Black),
            sf::Vertex(sf::Vector2f(imagePos.x - 2, screenY), sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
        
        // Texte de graduation (seulement pour les ticks majeurs ou si assez d'espace)
        if (fontLoaded && (isMajorTick || screenStep > 60)) {
            sf::Text text;
            text.setFont(font);
            text.setString(formatValue(imageY, step));
            text.setCharacterSize(9);
            text.setFillColor(rulerTextColor);
            text.setRotation(90);
            
            sf::FloatRect textBounds = text.getLocalBounds();
            text.setPosition(imagePos.x - RULER_SIZE + 12, screenY - 5);
            
            // Ne dessiner que si le texte ne sort pas de la règle
            if (screenY - textBounds.width - 5 >= imagePos.y) {
                window.draw(text);
            }
        }
    }
    
    // Indicateur position souris
    if (mousePos.y >= imagePos.y && mousePos.y <= imagePos.y + imageSize.y) {
        sf::Vertex indicator[] = {
            sf::Vertex(sf::Vector2f(imagePos.x - RULER_SIZE, mousePos.y), crosshairColor),
            sf::Vertex(sf::Vector2f(imagePos.x, mousePos.y), crosshairColor)
        };
        window.draw(indicator, 2, sf::Lines);
    }
}

void Rulers::drawCrosshair(sf::RenderWindow& window, const sf::Vector2f& mousePos, 
                          const sf::Vector2f& imagePos, const sf::Vector2f& imageSize) {
    // Dessiner les lignes de crosshair dans l'image
    if (mousePos.x >= imagePos.x && mousePos.x <= imagePos.x + imageSize.x &&
        mousePos.y >= imagePos.y && mousePos.y <= imagePos.y + imageSize.y) {
        
        // Ligne verticale
        sf::Vertex vLine[] = {
            sf::Vertex(sf::Vector2f(mousePos.x, imagePos.y), crosshairColor),
            sf::Vertex(sf::Vector2f(mousePos.x, imagePos.y + imageSize.y), crosshairColor)
        };
        window.draw(vLine, 2, sf::Lines);
        
        // Ligne horizontale
        sf::Vertex hLine[] = {
            sf::Vertex(sf::Vector2f(imagePos.x, mousePos.y), crosshairColor),
            sf::Vertex(sf::Vector2f(imagePos.x + imageSize.x, mousePos.y), crosshairColor)
        };
        window.draw(hLine, 2, sf::Lines);
    }
}
