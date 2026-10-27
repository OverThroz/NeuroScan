/*
** EPITA - SAE S3 - 2026
** Projet NeuroScan
** Auteur : Maxime
** Role   : Pont entre le Solver et l'interface graphique (Calcul de rendu)
*/

#ifndef RENDER_H
#define RENDER_H

#include "wordposition.h"

/*
** Boîte englobante (Bounding Box) utilisée par l'interface graphique (Louis)
** pour dessiner les ellipses. On travaille en double pour la précision.
** L'angle est en radians : 0 pour l'horizontal, PI/4 pour une diagonale, etc.
*/
typedef struct {
    double x, y;       // Coin supérieur gauche
    double w, h;       // Largeur et hauteur de la boîte
    double angle;      // Rotation en radians
} BBox;

/*
** Représente la position de la grille dans l'image source.
** L'OCR doit fournir ces infos (où commence la grille, quelle taille font les cases).
*/
typedef struct {
    double origin_x;   // Coordonnée X du coin haut-gauche de la grille
    double origin_y;   // Coordonnée Y du coin haut-gauche de la grille
    double cell_w;     // Largeur d'une case en pixels
    double cell_h;     // Hauteur d'une case en pixels
} GridGeom;

/*
** Calcule la boîte englobante en pixels pour un mot trouvé.
** Calcule le centre du mot, la distance entre les lettres (pour la largeur),
** et utilise atan2() pour déterminer l'angle de rotation de l'ellipse.
** C'est ce qui permet à Louis de dessiner un ovale même si le mot est en diagonale.
*/
BBox calc_word_box(WordPosition pos, GridGeom geom);

#endif // RENDER_H