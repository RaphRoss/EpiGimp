# 🗺️ EpiGimp - Roadmap de développement

## Objectif
Développer un éditeur d'images qui ressemble au maximum à GIMP avec toutes ses fonctionnalités principales.

---

## ✅ NIVEAU 1 - OUTILS DE DESSIN ESSENTIELS (Priorité Haute)

### 1.1 Système de couleurs ✅
- [x] **ColorManager** : Gestionnaire de couleurs singleton
  - [x] Couleur de premier plan
  - [x] Couleur d'arrière-plan
  - [x] Historique des 20 dernières couleurs
  - [x] Fonction swap (échange)
  - [x] Reset aux valeurs par défaut

- [x] **Sélecteur de couleur (ColorPicker)**
  - [x] Widget HSV (Hue + SV)
  - [x] Sliders RGB + Alpha
  - [x] Historique des couleurs cliquable
  - [x] Toggle avec touche `C`
  - [x] Échange premier plan/arrière-plan avec `X`
  - [x] Reset avec touche `D`

- [x] **Outil Pipette (Eyedropper)**
  - [x] Prélèvement de couleur sur l'image
  - [x] Mise à jour automatique du premier plan

### 1.2 Outils de pinceau avancés ✅
- [x] **Pinceau (Brush)**
  - [x] Taille variable (1-500px)
  - [x] Opacité réglable (0-100%)
  - [x] Dureté du pinceau (bords doux/durs)
  - [x] Interpolation pour traits lisses
  - [x] Utilisation couleur de premier plan

- [x] **Gomme (Eraser)**
  - [x] Taille variable
  - [x] Opacité réglable
  - [x] Dureté réglable
  - [x] Efface vers transparence ou couleur d'arrière-plan

- [x] **Aérographe (Airbrush)**
  - [x] Accumulation de couleur progressive
  - [x] Débit réglable

### 1.3 Outils de remplissage ✅
- [x] **Pot de peinture (Bucket Fill)**
  - [x] Algorithme flood-fill (BFS)
  - [x] Seuil de tolérance réglable
  - [x] Remplissage de toute la sélection
  - [x] Respect des limites de sélection

- [x] **Dégradé (Gradient)**
  - [x] Dégradés linéaires
  - [x] Dégradés radiaux
  - [x] Avant-plan → Arrière-plan
  - [x] Avant-plan → Transparent
  - [ ] Dégradés personnalisés
  - [ ] Plus de types (conique, carré)

### 1.4 Interface utilisateur ✅
- [x] **Panneau d'options des outils (ToolOptionsPanel)**
  - [x] Options contextuelles selon l'outil
  - [x] Sliders interactifs
  - [x] Mise à jour en temps réel

---

## 📋 NIVEAU 2 - SYSTÈME DE CALQUES (Priorité Haute)

### 2.1 Gestion basique des calques
- [ ] **LayerManager**
  - [ ] Structure de données pour gérer plusieurs calques
  - [ ] Calque actif
  - [ ] Ordre des calques (z-index)
  - [ ] Rendu composite de tous les calques

- [ ] **Panneau des calques (LayerPanel)**
  - [ ] Liste des calques avec miniatures
  - [ ] Nom éditable pour chaque calque
  - [ ] Indicateur de visibilité (œil)
  - [ ] Calque actif surligné
  - [ ] Défilement si beaucoup de calques

### 2.2 Opérations sur calques
- [ ] **Création et suppression**
  - [ ] Créer nouveau calque
  - [ ] Dupliquer calque
  - [ ] Supprimer calque
  - [ ] Fusionner calques (merge down)
  - [ ] Aplatir l'image (flatten)

- [ ] **Organisation**
  - [ ] Réorganiser par glisser-déposer
  - [ ] Monter/Descendre un calque
  - [ ] Groupes de calques (dossiers)
  - [ ] Renommer calques

### 2.3 Propriétés des calques
- [ ] **Modes de fusion (Blend Modes)**
  - [ ] Normal
  - [ ] Multiply
  - [ ] Screen
  - [ ] Overlay
  - [ ] Addition
  - [ ] Subtract
  - [ ] Difference
  - [ ] Lighten
  - [ ] Darken
  - [ ] Color Dodge / Burn

- [ ] **Opacité et verrouillage**
  - [ ] Opacité globale du calque (0-100%)
  - [ ] Verrouillage alpha (transparence)
  - [ ] Verrouillage position
  - [ ] Verrouillage pixels

- [ ] **Masques de calque**
  - [ ] Créer masque alpha
  - [ ] Édition du masque
  - [ ] Lier/Délier masque
  - [ ] Appliquer/Supprimer masque
  - [ ] Masque depuis sélection

---

## ✂️ NIVEAU 3 - SÉLECTIONS AVANCÉES (Priorité Haute)

### 3.1 Nouveaux outils de sélection
- [ ] **Sélection elliptique/circulaire**
  - [ ] Outil ellipse
  - [ ] Maintenir Shift pour cercle parfait
  - [ ] Modes : remplacer, ajouter, soustraire, intersection

- [ ] **Sélection à main levée (Lasso)**
  - [ ] Tracé libre pour sélectionner
  - [ ] Lasso polygonal (clic par clic)
  - [ ] Lasso magnétique (snap aux bords)

- [ ] **Sélection par couleur (Magic Wand)**
  - [ ] Sélection de zones de couleur similaire
  - [ ] Tolérance réglable
  - [ ] Sélection contiguë ou globale

- [ ] **Sélection par chemins (Bézier)**
  - [ ] Outil plume pour tracés précis
  - [ ] Courbes de Bézier
  - [ ] Conversion chemin → sélection

### 3.2 Opérations sur sélections
- [ ] **Modifications de sélection**
  - [ ] Agrandir (Grow)
  - [ ] Rétrécir (Shrink)
  - [ ] Adoucir les bords (Feather)
  - [ ] Inverser la sélection (`Ctrl+I`)
  - [ ] Bordure de sélection (Border)
  - [ ] Arrondir les coins

- [ ] **Transformer la sélection**
  - [ ] Déplacer uniquement le contour
  - [ ] Rotation de la sélection
  - [ ] Échelle de la sélection
  - [ ] Distorsion

- [ ] **Sauvegarder/Charger**
  - [ ] Sauvegarder sélection en canal
  - [ ] Charger sélection depuis canal
  - [ ] Sélections nommées

---

## 🔧 NIVEAU 4 - TRANSFORMATIONS (Priorité Moyenne)

### 4.1 Transformations de base
- [x] Rotation 90°, 180°, 270°
- [x] Retournement horizontal/vertical
- [ ] **Rotation libre**
  - [ ] Angle personnalisé
  - [ ] Point de pivot déplaçable
  - [ ] Prévisualisation en temps réel

- [ ] **Échelle/Redimensionnement**
  - [ ] Redimensionner image entière
  - [ ] Redimensionner calque
  - [ ] Conserver proportions
  - [ ] Interpolation (Nearest, Bilinear, Bicubic)
  - [ ] Redimensionner canevas

### 4.2 Transformations avancées
- [ ] **Perspective**
  - [ ] Transformation en perspective
  - [ ] 4 points de contrôle
  - [ ] Grille de prévisualisation

- [ ] **Cisaillement (Shear)**
  - [ ] Inclinaison horizontale
  - [ ] Inclinaison verticale

- [ ] **Déformation libre (Warp)**
  - [ ] Grille de déformation
  - [ ] Déplacement de points de contrôle
  - [ ] Modes : Move, Grow, Shrink, Twirl

- [~] Recadrage (Crop) *(implementer mais ne fonctionne pas)*

### 4.3 Transformations automatiques
- [ ] **Corrections automatiques**
  - [ ] Auto-rotation selon EXIF
  - [ ] Correction de perspective
  - [ ] Redresser l'horizon

---

## 🎭 NIVEAU 5 - FILTRES ET EFFETS (Priorité Moyenne)

### 5.1 Filtres de flou
- [ ] **Flou gaussien**
  - [ ] Rayon réglable
  - [ ] Aperçu en temps réel
  - [ ] Application sur calque ou sélection

- [ ] **Flou de mouvement**
  - [ ] Direction et distance
  - [ ] Angle réglable

- [ ] **Flou radial**
  - [ ] Centré sur un point
  - [ ] Zoom blur / Rotation blur

- [ ] **Flou de profondeur de champ**

### 5.2 Ajustements de couleurs
- [ ] **Luminosité/Contraste**
  - [ ] Curseurs de réglage
  - [ ] Aperçu temps réel
  - [ ] Auto-ajustement

- [ ] **Teinte/Saturation/Valeur**
  - [ ] HSV adjustment
  - [ ] Colorisation
  - [ ] Par canal de couleur

- [ ] **Niveaux (Levels)**
  - [ ] Histogramme
  - [ ] Points noir/gris/blanc
  - [ ] Niveaux d'entrée et sortie
  - [ ] Auto-niveaux

- [ ] **Courbes (Curves)**
  - [ ] Courbe de tonalité
  - [ ] Canaux RGB séparés
  - [ ] Points de contrôle
  - [ ] Presets (Linear, Auto, etc.)

- [ ] **Balance des couleurs**
  - [ ] Cyan-Rouge
  - [ ] Magenta-Vert
  - [ ] Jaune-Bleu
  - [ ] Pour Ombres/Tons moyens/Hautes lumières

- [ ] **Désaturation**
  - [ ] Convertir en niveaux de gris
  - [ ] Méthodes : Average, Luminosity, Desaturate

- [ ] **Inverser les couleurs**
- [ ] **Postériser**
- [ ] **Seuil (Threshold)**

### 5.3 Filtres artistiques
- [ ] **Pixeliser (Pixelate)**
- [ ] **Mosaïque**
- [ ] **Peinture à l'huile**
- [ ] **Détection de bords**
- [ ] **Emboss (Relief)**
- [ ] **Cartoon**
- [ ] **Bruit**
  - [ ] Ajouter du bruit (RGB, HSV)
  - [ ] Réduire le bruit

### 5.4 Filtres de netteté
- [ ] **Netteté (Sharpen)**
  - [ ] Rayon et intensité
  - [ ] Unsharp Mask (Accentuation)

- [ ] **High Pass Filter**

### 5.5 Filtres de distorsion
- [ ] **Ondulation (Ripple)**
- [ ] **Tourbillon (Twirl)**
- [ ] **Sphérisation**
- [ ] **Lens Distortion**

---

## 📝 NIVEAU 6 - OUTILS DE TEXTE (Priorité Haute)

### 6.1 Outil texte basique
- [ ] **Insertion de texte**
  - [ ] Clic pour placer texte
  - [ ] Éditeur de texte inline
  - [ ] Police sélectionnable
  - [ ] Taille de police
  - [ ] Couleur du texte

### 6.2 Formatage avancé
- [ ] **Style de texte**
  - [ ] Gras, Italique, Souligné
  - [ ] Espacement des lettres (kerning)
  - [ ] Espacement des lignes (leading)
  - [ ] Alignement (gauche, centre, droite, justifié)

- [ ] **Boîte de texte**
  - [ ] Zone de texte redimensionnable
  - [ ] Retour à la ligne automatique
  - [ ] Remplissage et marges

- [ ] **Effets de texte**
  - [ ] Contour (stroke)
  - [ ] Ombre portée
  - [ ] Texte le long d'un chemin
  - [ ] Texte en 3D

---

## 🖼️ NIVEAU 7 - FONCTIONNALITÉS D'IMAGE (Priorité Basse-Moyenne)

### 7.1 Gestion d'image
- [ ] **Taille de l'image**
  - [ ] Redimensionner canevas
  - [ ] Échantillonnage (None, Linear, Cubic, Lanczos)
  - [ ] Conserver ratio

- [ ] **Taille du canevas**
  - [ ] Modifier dimensions sans redimensionner contenu
  - [ ] Ancrage du contenu (9 positions)
  - [ ] Remplissage de l'espace vide

- [ ] **Aplatir l'image**
- [ ] **Mode couleur**
  - [ ] RVB
  - [ ] Niveaux de gris
  - [ ] Indexé (palette)
  - [ ] CMYK

### 7.2 Corrections automatiques
- [ ] **Auto-niveaux**
- [ ] **Auto-contraste**
- [ ] **Auto-balance des blancs**
- [ ] **Égalisation d'histogramme**
- [ ] **Correction gamma**

### 7.3 Informations sur l'image
- [ ] **Propriétés**
  - [ ] Dimensions
  - [ ] Résolution (DPI)
  - [ ] Mode couleur
  - [ ] Taille du fichier
  - [ ] Métadonnées EXIF

---

## 🔍 NIVEAU 8 - OUTILS DE RETOUCHE (Priorité Moyenne)

### 8.1 Outils de clonage et réparation
- [ ] **Tampon de clonage (Clone Stamp)**
  - [ ] Ctrl+Clic pour définir source
  - [ ] Clonage avec alignement
  - [ ] Taille et opacité variables

- [ ] **Outil de correction (Healing Brush)**
  - [ ] Mélange intelligent avec environnement
  - [ ] Correction de texture

- [ ] **Correcteur localisé**
  - [ ] Correction automatique de zone
  - [ ] Détection de contenu

- [ ] **Pièce (Patch Tool)**
  - [ ] Sélectionner et glisser pour réparer

### 8.2 Outils de retouche
- [ ] **Doigt (Smudge)**
  - [ ] Étalement de pixels
  - [ ] Force réglable

- [ ] **Netteté/Flou localisé**
  - [ ] Pinceau de netteté
  - [ ] Pinceau de flou

- [ ] **Éclaircir/Assombrir (Dodge/Burn)**
  - [ ] Exposition localisée
  - [ ] Pour ombres, tons moyens, hautes lumières

- [ ] **Éponge (Saturation)**
  - [ ] Augmenter/diminuer saturation localement

### 8.3 Corrections de la peau
- [ ] **Yeux rouges**
- [ ] **Lissage de peau**
- [ ] **Blanchiment des dents**

---

## 💾 NIVEAU 9 - FORMATS ET EXPORT (Priorité Haute)

### 9.1 Support de formats
- [x] PNG
- [ ] **JPEG**
  - [ ] Qualité réglable (0-100)
  - [ ] Optimisation progressive
  - [ ] Sous-échantillonnage chroma

- [ ] **BMP**
- [ ] **GIF**
  - [ ] Support transparence
  - [ ] Animation GIF
  - [ ] Palette optimisée

- [ ] **TIFF**
  - [ ] Non compressé
  - [ ] LZW compression
  - [ ] Support calques

- [ ] **WebP**
  - [ ] Format moderne
  - [ ] Lossy et Lossless

- [ ] **Format natif (.xcf / .egimp)**
  - [ ] Sauvegarde des calques
  - [ ] Sauvegarde de l'historique
  - [ ] Sauvegarde des sélections
  - [ ] Métadonnées de projet

### 9.2 Export avancé
- [ ] **Export pour web**
  - [ ] Optimisation de taille
  - [ ] Prévisualisation avec comparaison
  - [ ] Dimensionnement responsive

- [ ] **Batch processing**
  - [ ] Appliquer action à plusieurs images
  - [ ] Scripts d'automatisation
  - [ ] Conversion en masse

- [ ] **Impression**
  - [ ] Aperçu avant impression
  - [ ] Gestion des marges
  - [ ] Résolution d'impression

---

## ⚙️ NIVEAU 10 - PRÉFÉRENCES ET CONFIGURATION (Priorité Basse)

### 10.1 Préférences utilisateur
- [ ] **Interface**
  - [ ] Thème clair/sombre
  - [ ] Taille des icônes
  - [ ] Langue (i18n)
  - [ ] Dispositions d'interface personnalisables

- [ ] **Grille et guides**
  - [ ] Espacement de grille personnalisé
  - [ ] Couleur de grille
  - [ ] Guides magnétiques (snap to guides)
  - [ ] Guides intelligents

- [ ] **Performance**
  - [ ] Limite de RAM
  - [ ] Nombre d'étapes d'annulation
  - [ ] Taille du cache
  - [ ] Utilisation GPU

- [ ] **Outils par défaut**
  - [ ] Sauvegarder préférences d'outils
  - [ ] Outil au démarrage
  - [ ] Réinitialiser paramètres

### 10.2 Raccourcis personnalisables
- [ ] **Éditeur de raccourcis**
  - [ ] Assigner touches aux actions
  - [ ] Profils de raccourcis
  - [ ] Import/Export raccourcis
  - [ ] Réinitialiser par défaut

### 10.3 Extensions et plugins
- [ ] **Gestionnaire de plugins**
  - [ ] Liste des plugins installés
  - [ ] Activation/Désactivation
  - [ ] Mise à jour automatique

---

## 🎯 NIVEAU 11 - FONCTIONNALITÉS AVANCÉES (Priorité Basse)

### 11.1 Plugins et scripts
- [ ] **Système de plugins**
  - [ ] API pour extensions C++
  - [ ] Chargement dynamique (.so/.dll)
  - [ ] Interface plugin standard

- [ ] **Scripts**
  - [ ] Support Python/Lua
  - [ ] Automatisation
  - [ ] Script-Fu basique
  - [ ] Enregistrer actions en script

### 11.2 Chemins et vecteurs
- [ ] **Outil plume/chemins**
  - [ ] Tracés vectoriels
  - [ ] Courbes de Bézier
  - [ ] Conversion chemin → sélection
  - [ ] Tracé le long d'un chemin
  - [ ] Export SVG

- [ ] **Panneau chemins**
  - [ ] Liste des chemins
  - [ ] Visibilité
  - [ ] Édition

### 11.3 Canaux
- [ ] **Panneau des canaux**
  - [ ] Rouge, Vert, Bleu séparés
  - [ ] Canal alpha
  - [ ] Canaux personnalisés (spot colors)
  - [ ] Édition par canal

### 11.4 Historique avancé
- [ ] **Panneau historique**
  - [ ] Liste des actions
  - [ ] Aperçu visuel
  - [ ] Retour à n'importe quelle étape
  - [ ] Branches d'historique

### 11.5 Outils 3D
- [ ] **Texte 3D**
- [ ] **Formes 3D basiques**
- [ ] **Éclairage et ombres**

---

## 📊 PHASES DE DÉVELOPPEMENT RECOMMANDÉES

### 🟢 Phase actuelle : Niveau 1 (EN COURS)
**Durée** : ~
**Objectif** : Outils de dessin de base fonctionnels

### 🔵 Phase 2 : Niveau 2 - Calques (EN COURS)
**Durée estimée** : 2-3 semaines  
**Objectif** : Système de calques complet avec modes de fusion

**Priorités** :
1. LayerManager et structure de données
2. LayerPanel UI
3. Opérations de base (créer, supprimer, dupliquer)
4. Modes de fusion essentiels
5. Opacité par calque

### 🔵 Phase 3 : Niveau 3 - Sélections
**Durée estimée** : 1-2 semaines  
**Objectif** : Outils de sélection avancés

**Priorités** :
1. Sélection elliptique
2. Lasso libre
3. Magic Wand
4. Opérations sur sélections

### 🟡 Phase 4 : Niveau 5 - Filtres de base
**Durée estimée** : 2 semaines  
**Objectif** : Filtres les plus utilisés

**Priorités** :
1. Flou gaussien
2. Luminosité/Contraste
3. Teinte/Saturation
4. Niveaux
5. Netteté

### 🟡 Phase 5 : Niveau 4 - Transformations
**Durée estimée** : 1 semaine  
**Objectif** : Transformations libres

**Priorités** :
1. Rotation libre
2. Échelle
3. Perspective

### 🟡 Phase 6 : Niveau 6 - Texte
**Durée estimée** : 1 semaine  
**Objectif** : Outil texte fonctionnel

### 🟠 Phase 7 : Niveaux 8-9 - Retouche et Formats
**Durée estimée** : 2 semaines  
**Objectif** : Outils professionnels

### 🔴 Phase 8 : Niveaux 10-11 - Polish et avancé
**Durée estimée** : 2-3 semaines  
**Objectif** : Finitions et fonctionnalités pro

---

## 🎯 OBJECTIFS À COURT TERME

### Cette semaine
- [ ] Débuter le système de calques
- [ ] Implémenter LayerManager de base
- [ ] Créer LayerPanel UI

### Ce mois
- [ ] Finir le système de calques
- [ ] Ajouter modes de fusion
- [ ] Implémenter sélections avancées

### Ce trimestre
- [ ] Niveaux 2, 3, 4, 5 complétés
- [ ] EpiGimp utilisable pour édition basique
- [ ] Support multi-formats

---

## 📈 MÉTRIQUES DE PROGRÈS

### Fonctionnalités totales : ~150+
- ✅ **Complétées** : 23 (15%)
- 🔄 **En cours** : 0 (0%)
- ⏳ **Planifiées** : 127+ (85%)

### Par niveau :
- ✅ **Niveau 1** : 0% (23/23)
- ⏳ **Niveau 2** : 0% (0/20)
- ⏳ **Niveau 3** : 0% (0/18)
- ⏳ **Niveau 4** : 15% (3/20)
- ⏳ **Niveau 5** : 0% (0/30)
- ⏳ **Niveau 6** : 0% (0/10)
- ⏳ **Niveau 7** : 0% (0/15)
- ⏳ **Niveau 8** : 0% (0/15)
- ⏳ **Niveau 9** : 10% (1/10)
- ⏳ **Niveau 10** : 0% (0/15)
- ⏳ **Niveau 11** : 0% (0/15)

---

## 🚀 DERNIÈRES MISES À JOUR

**Version actuelle** : EpiGimp v2.0 (Niveau 0)  
**Dernière mise à jour** : 23 octobre 2025
