/*
** EPITA - SAE S3 - 2026
** Project NeuroScan
** Author : Ibrahim
** Role   : Image preprocessing (Grayscale, Binarization, Rotation)
*/

#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <SDL2/SDL.h>

/**
 * Converts an image to grayscale.
 * Returns a new surface (remember to free it with SDL_FreeSurface).
 */
SDL_Surface *convert_to_grayscale(SDL_Surface *image);

/**
 * Binarizes the image (strict black and white).
 * Uses Otsu's method to automatically find the optimal threshold.
 * Returns a new binary surface.
 */
SDL_Surface *binarize_image(SDL_Surface *image);

/**
 * Straightens the image by applying a rotation of a given angle.
 * angle_degrees: angle in degrees (e.g., 2.5, -1.0).
 * Returns a new surface (size may change).
 */
SDL_Surface *rotate_image(SDL_Surface *image, double angle_degrees);

#endif // PREPROCESSING_H
