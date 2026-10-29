#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <stddef.h>
#include <SDL2/SDL.h>

/*
 * Segmentation module.
 * Works on a binarized ARGB8888 surface (output of binarize_image):
 * black pixels are text or grid lines, white pixels are background.
 */

/* Axis-aligned rectangle in image pixels. */
struct box
{
    int x;
    int y;
    int w;
    int h;
};


/*
 * Find the bounding box of the grid.
 * Returns 0 on success, -1 if no grid is found.
 */
int seg_find_grid(SDL_Surface *bw, struct box *grid);

/*
 * Split the grid into cells. Stores the number of rows and columns.
 * Returns a malloc'ed array of rows * cols boxes (row-major order),
 * or NULL on failure. The caller frees it with free().
 */
struct box *seg_grid_cells(SDL_Surface *bw, struct box grid,
                           int *rows, int *cols);


/*
 * Find the bounding box of the word list, outside the grid.
 * Returns 0 on success, -1 if nothing is found.
 */
int seg_find_list(SDL_Surface *bw, struct box grid, struct box *list);

/*
 * Split 'area' into horizontal bands separated by empty rows
 * (one band per word). Returns a malloc'ed array and stores its
 * size in *count, or NULL on failure. The caller frees it.
 */
struct box *seg_split_rows(SDL_Surface *bw, struct box area, size_t *count);

/*
 * Split 'area' into vertical bands separated by empty columns
 * (one band per letter). Same contract as seg_split_rows.
 */
struct box *seg_split_cols(SDL_Surface *bw, struct box area, size_t *count);

/* ---- Shared ---- */

/*
 * Copy the 'b' region of 'src' into a new image and save it as PNG.
 * Returns 0 on success, -1 on failure.
 */
int seg_save_box(SDL_Surface *src, struct box b, const char *path);

#endif /* SEGMENTATION_H */
