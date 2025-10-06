# EpiGimp

EpiGimp est un projet de simulation GIMP proposé par Epitech, développé en C++ avec SFML.

## 🚀 Fonctionnalités de navigation et visualisation

### 🛠️ Outils disponibles
- **Sélection** : Outil de sélection rectangulaire avec marching ants animées
- **Crayon** : Outil de dessin basique avec historique
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

### 🗂️ Système d'onglets
- **Onglets multiples** : Chaque image s'ouvre dans son propre onglet
- **Indicateur de modifications** : Point orange sur les onglets modifiés
- **Fermeture sécurisée** : Demande de sauvegarde avant fermeture d'un onglet modifié
- **Navigation entre onglets** : Clic pour changer d'onglet
- **Bouton de fermeture** : X sur chaque onglet pour fermeture rapide

### ↩️ Historique et annulation
- **Annulation** : `Ctrl+Z` ou menu Edit → Undo
- **Rétablir** : `Ctrl+Y` ou menu Edit → Redo
- **Historique limité** : 50 actions par défaut
- **Pattern Command** : Architecture propre pour toutes les actions

### ✂️ Système de sélection
- **Sélection rectangulaire** : Outil de sélection principal
- **Marching ants** : Animation de sélection comme dans GIMP
- **Modes de sélection** : Remplacer, ajouter, soustraire, intersection
- **Raccourcis sélection** :
  - `Ctrl+A` : Sélectionner tout
  - `Ctrl+Shift+A` : Désélectionner tout

### 🎮 Raccourcis clavier
- `Ctrl+N` : Nouvelle image
- `Ctrl+O` : Ouvrir une image
- `Ctrl+S` : Sauvegarder
- `Ctrl+W` : Fermer l'onglet actuel
- `Ctrl+Tab` : Image suivante
- `Ctrl+Z` : Annuler
- `Ctrl+Y` : Rétablir
- `Ctrl+A` : Sélectionner tout
- `Ctrl+Shift+A` : Désélectionner tout
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
✅ Navigation multi-images avec onglets  
✅ Raccourcis clavier complets  
✅ Système d'historique avec annulation/rétablissement  
✅ Outil de sélection rectangulaire avec marching ants  
✅ Gestion des modifications et sauvegarde sécurisée  
✅ Architecture Pattern Command pour l'extensibilité  

### 🔄 Prochaines améliorations prévues
- Système de couleurs
- Taille de pinceau variable
- Plus d'outils (gomme, pot de peinture, etc.)
- Système de calques
- Sélection elliptique et lasso
- Outils de transformation
