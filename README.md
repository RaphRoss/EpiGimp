# EpiGimp

EpiGimp est un projet de simulation GIMP proposé par Epitech, développé en C++ avec SFML.

## 🚀 Nouvelles fonctionnalités de navigation et visualisation

### 🛠️ Outils disponibles
- **Crayon** : Outil de dessin basique
- **Main (Pan)** : Déplace la vue de l'image (clic et glisser)

### 🎯 Navigation
- **Image ajustée automatiquement** : Les nouvelles images sont automatiquement ajustées à la taille de la vue
- **Déplacement libre** : L'image peut être déplacée librement avec l'outil Main
- **Zoom centré sur curseur** : Molette de souris zoome vers le point sous le curseur
- **Outil Main temporaire** : Maintenir `Espace` pour activer temporairement l'outil main
- **Raccourcis zoom** :
  - `+` ou `=` : Zoom avant
  - `-` : Zoom arrière
  - `0` : Reset zoom (100%)
- **Positionnement** :
  - `H` : Centrer l'image dans la vue
  - `F` : Ajuster l'image à la taille de la vue (manuel)

### 📏 Aides visuelles
- **Grille** : Affichage d'une grille de guidage
  - Toggle avec `Ctrl+G` ou menu View → Toggle Grid
- **Règles** : Règles horizontales et verticales avec graduations
  - Toggle avec `Ctrl+R` ou menu View → Toggle Rulers
  - Indicateurs de position de la souris sur les règles
  - Crosshair dans l'image

### 📊 Barre d'état
- Position de la souris dans l'image (en pixels)
- Niveau de zoom actuel (en %)
- Informations sur l'image courante (nom et dimensions)
- Outil actuellement sélectionné

### 🎮 Raccourcis clavier
- `Ctrl+N` : Nouvelle image
- `Ctrl+O` : Ouvrir une image
- `Ctrl+S` : Sauvegarder
- `Ctrl+Tab` : Image suivante
- `Ctrl+G` : Toggle grille
- `Ctrl+R` : Toggle règles
- `Espace` : Outil main temporaire
- `H` : Centrer l'image
- `F` : Ajuster l'image à la vue

### 📋 Fonctionnalités GIMP implémentées
✅ Ajustement automatique des nouvelles images à la vue  
✅ Déplacement libre de l'image  
✅ Zoom centré sur curseur  
✅ Outil main (pan)  
✅ Positionnement intelligent (centrage/ajustement)  
✅ Grille configurable  
✅ Règles adaptatives avec graduations intelligentes  
✅ Formatage intelligent des règles (K pour grandes valeurs)  
✅ Graduations majeures/mineures adaptatives  
✅ Barre d'état informative  
✅ Crosshair de position  
✅ Navigation multi-images  
✅ Raccourcis clavier  

### 🔄 Prochaines améliorations prévues
- Système de couleurs
- Taille de pinceau variable
- Plus d'outils (gomme, pot de peinture, etc.)
- Système de calques
- Historique d'annulation
