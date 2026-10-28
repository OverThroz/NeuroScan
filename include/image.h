#ifndef IMAGE_H
#define IMAGE_H

#include <SDL2/SDL.h>

/*
 * Image input/output module.
 * Loads images in any format supported by SDL_image, converts them to
 * ARGB8888 so every module works on the same pixel layout, and saves
 * them as PNG.
 */



/*
 * Initialize SDL_image with PNG and JPG support.
 * Must be called once before image_load or image_save.
 * Returns 0 on success, -1 on failure.
 */
int image_init(void);

/*
 * Load the image at 'path' and convert it to SDL_PIXELFORMAT_ARGB8888.
 * Returns a new surface, or NULL on failure.
 * The caller owns the surface and must free it with SDL_FreeSurface().
 */
SDL_Surface *image_load(const char *path);

/*
 * Save 'surface' as a PNG file at 'path'.
 * Returns 0 on success, -1 on failure.
 */
int image_save(SDL_Surface *surface, const char *path);

/*
 * Shut down SDL_image. Call once, after the last image_load/image_save.
 */
void image_quit(void);


#endif /* IMAGE_H */
