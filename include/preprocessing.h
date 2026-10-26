#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <SDL2/SDL.h>

/**
 * Convertit une image en niveaux de gris.
 * Retourne une nouvelle surface (pense à la libérer avec SDL_FreeSurface).
 */
SDL_Surface* convert_to_grayscale(SDL_Surface *image);

/**
 * Binarise l'image (Noir & Blanc strict).
 * Utilise la méthode d'Otsu pour trouver automatiquement le meilleur seuil.
 * Retourne une nouvelle surface binaire.
 */
SDL_Surface* binarize_image(SDL_Surface *image);

/**
 * Redresse l'image en appliquant une rotation d'un certain angle.
 * angle_degrees : angle en degrés (ex: 2.5, -1.0).
 * Retourne une nouvelle surface (la taille peut changer).
 */
SDL_Surface* rotate_image(SDL_Surface *image, double angle_degrees);

#endif // PREPROCESSING_H