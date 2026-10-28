#include "preprocessing.h"
#include <stdlib.h>
#include <math.h>

//M_PI is not part of ISO C: define it when -std=c11 hides it.
#ifndef M_PI
#define M_PI 3.14159265358979323846

// --- 1. CONVERSION EN NIVEAUX DE GRIS ---
SDL_Surface* convert_to_grayscale(SDL_Surface *image) {
    // On s'assure d'avoir un format de pixel standard pour travailler facilement
    SDL_Surface *result = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);
    if (!result) return NULL;

    Uint32 *pixels = (Uint32 *)result->pixels;
    int width = result->w;
    int height = result->h;

    for (int i = 0; i < width * height; i++) {
        Uint8 r, g, b, a;
        SDL_GetRGBA(pixels[i], result->format, &r, &g, &b, &a);
        
        // Formule standard de luminance (perception humaine)
        Uint8 gray = (Uint8)(0.299 * r + 0.587 * g + 0.114 * b);
        
        pixels[i] = SDL_MapRGBA(result->format, gray, gray, gray, a);
    }
    return result;
}

// --- 2. BINARISATION (Méthode d'Otsu) ---
// L'algorithme d'Otsu trouve automatiquement le meilleur seuil pour séparer le texte du fond.
SDL_Surface* binarize_image(SDL_Surface *image) {
    SDL_Surface *gray = convert_to_grayscale(image);
    Uint32 *pixels = (Uint32 *)gray->pixels;
    int total_pixels = gray->w * gray->h;

    // 1. Calcul de l'histogramme
    int histogram[256] = {0};
    for (int i = 0; i < total_pixels; i++) {
        Uint8 r, g, b, a;
        SDL_GetRGBA(pixels[i], gray->format, &r, &g, &b, &a);
        histogram[r]++; // r == g == b car c'est du gris
    }

    // 2. Calcul du seuil optimal avec Otsu
    float sum = 0;
    for (int i = 0; i < 256; i++) sum += i * histogram[i];

    float sumB = 0, varMax = 0;
    int wB = 0, threshold = 0;

    for (int t = 0; t < 256; t++) {
        wB += histogram[t];
        if (wB == 0) continue;
        int wF = total_pixels - wB;
        if (wF == 0) break;

        sumB += (float)(t * histogram[t]);
        float mB = sumB / wB;
        float mF = (sum - sumB) / wF;
        float varBetween = (float)wB * (float)wF * (mB - mF) * (mB - mF);

        if (varBetween > varMax) {
            varMax = varBetween;
            threshold = t;
        }
    }

    // 3. Application du seuil (Noir ou Blanc strict)
    for (int i = 0; i < total_pixels; i++) {
        Uint8 r, g, b, a;
        SDL_GetRGBA(pixels[i], gray->format, &r, &g, &b, &a);
        
        // Si le pixel est plus clair que le seuil, on le met en blanc (255), sinon en noir (0)
        Uint8 val = (r > threshold) ? 255 : 0;
        
        pixels[i] = SDL_MapRGBA(gray->format, val, val, val, a);
    }

    return gray;
}

// --- 3. ROTATION DE L'IMAGE ---
// Redresse l'image en cas de scan légèrement penché.
SDL_Surface* rotate_image(SDL_Surface *image, double angle_degrees) {
    // Conversion de l'angle en radians
    double angle = angle_degrees * M_PI / 180.0;
    double cos_a = cos(angle);
    double sin_a = sin(angle);

    int w = image->w;
    int h = image->h;

    // Calcul des nouvelles dimensions de l'image (pour ne pas couper les coins)
    int new_w = (int)(fabs(w * cos_a) + fabs(h * sin_a));
    int new_h = (int)(fabs(w * sin_a) + fabs(h * cos_a));

    SDL_Surface *result = SDL_CreateRGBSurfaceWithFormat(0, new_w, new_h, 32, SDL_PIXELFORMAT_ARGB8888);
    if (!result) return NULL;

    Uint32 *src_pixels = (Uint32 *)image->pixels;
    Uint32 *dst_pixels = (Uint32 *)result->pixels;

    // Centres des deux images
    double cx_src = w / 2.0;
    double cy_src = h / 2.0;
    double cx_dst = new_w / 2.0;
    double cy_dst = new_h / 2.0;

    // Remplissage du fond en blanc
    SDL_FillRect(result, NULL, SDL_MapRGBA(result->format, 255, 255, 255, 255));

    // Rotation inverse (pour chaque pixel de destination, on cherche sa provenance)
    for (int y = 0; y < new_h; y++) {
        for (int x = 0; x < new_w; x++) {
            // Coordonnées par rapport au centre
            double dx = x - cx_dst;
            double dy = y - cy_dst;

            // Application de la matrice de rotation inverse
            double src_x = cos_a * dx + sin_a * dy + cx_src;
            double src_y = -sin_a * dx + cos_a * dy + cy_src;

            // Vérification que le pixel source est bien dans l'image d'origine
            if (src_x >= 0 && src_x < w && src_y >= 0 && src_y < h) {
                int sx = (int)src_x;
                int sy = (int)src_y;
                dst_pixels[y * new_w + x] = src_pixels[sy * w + sx];
            }
        }
    }

    return result;
}

#endif
