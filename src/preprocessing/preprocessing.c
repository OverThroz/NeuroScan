#include "preprocessing.h"
#include <stdlib.h>
#include <math.h>

/* M_PI is not part of ISO C: define it when -std=c11 hides it. */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* --- 1. GRAYSCALE CONVERSION --- */
SDL_Surface *convert_to_grayscale(SDL_Surface *image)
{
    if (image == NULL) return NULL;

    SDL_Surface *result = SDL_ConvertSurfaceFormat(image,
                            SDL_PIXELFORMAT_ARGB8888, 0);
    if (result == NULL) return NULL;

    Uint32 *pixels = (Uint32 *)result->pixels;
    int width = result->w;
    int height = result->h;

    for (int i = 0; i < width * height; i++) {
        Uint8 r, g, b, a;
        SDL_GetRGBA(pixels[i], result->format, &r, &g, &b, &a);
        
        Uint8 gray = (Uint8)(0.299 * r + 0.587 * g + 0.114 * b);
        pixels[i] = SDL_MapRGBA(result->format, gray, gray, gray, a);
    }
    return result;
}

/* --- 2. BINARIZATION (Otsu's method) --- */
SDL_Surface *binarize_image(SDL_Surface *image)
{
    if (image == NULL) return NULL;

    SDL_Surface *gray = convert_to_grayscale(image);
    if (gray == NULL) return NULL;

    Uint32 *pixels = (Uint32 *)gray->pixels;
    int total_pixels = gray->w * gray->h;
    if (total_pixels == 0) return gray;

    int histogram[256] = {0};
    for (int i = 0; i < total_pixels; i++) {
        Uint8 r, g, b, a;
        SDL_GetRGBA(pixels[i], gray->format, &r, &g, &b, &a);
        histogram[r]++;
    }

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

    /* Force threshold to keep light grid lines */
    if (threshold < 180) {
        threshold = 180;
    }

    for (int i = 0; i < total_pixels; i++) {
        Uint8 r, g, b, a;
        SDL_GetRGBA(pixels[i], gray->format, &r, &g, &b, &a);
        
        Uint8 val = (r > threshold) ? 255 : 0;
        pixels[i] = SDL_MapRGBA(gray->format, val, val, val, a);
    }

    return gray;
}

/* --- 3. IMAGE ROTATION --- */
SDL_Surface *rotate_image(SDL_Surface *image, double angle_degrees)
{
    if (image == NULL) return NULL;

    double angle = angle_degrees * M_PI / 180.0;
    double cos_a = cos(angle);
    double sin_a = sin(angle);

    int w = image->w;
    int h = image->h;

    int new_w = (int)(fabs(w * cos_a) + fabs(h * sin_a));
    int new_h = (int)(fabs(w * sin_a) + fabs(h * cos_a));

    SDL_Surface *result = SDL_CreateRGBSurfaceWithFormat(0, new_w, new_h,
                                32, SDL_PIXELFORMAT_ARGB8888);
    if (result == NULL) return NULL;

    Uint32 *src_pixels = (Uint32 *)image->pixels;
    Uint32 *dst_pixels = (Uint32 *)result->pixels;

    double cx_src = w / 2.0;
    double cy_src = h / 2.0;
    double cx_dst = new_w / 2.0;
    double cy_dst = new_h / 2.0;

    SDL_FillRect(result, NULL, SDL_MapRGBA(result->format,
                 255, 255, 255, 255));

    for (int y = 0; y < new_h; y++) {
        for (int x = 0; x < new_w; x++) {
            double dx = x - cx_dst;
            double dy = y - cy_dst;

            double src_x = cos_a * dx + sin_a * dy + cx_src;
            double src_y = -sin_a * dx + cos_a * dy + cy_src;

            if (src_x >= 0 && src_x < w && src_y >= 0 && src_y < h) {
                int sx = (int)src_x;
                int sy = (int)src_y;
                dst_pixels[y * new_w + x] = src_pixels[sy * w + sx];
            }
        }
    }
    return result;
}