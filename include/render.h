#ifndef RENDER_H
#define RENDER_H

#include "wordposition.h"

/* Bounding box for drawing on the image. */
typedef struct {
    double x, y;       /* Top-left corner */
    double w, h;       /* Width and height */
    double angle;      /* Rotation in radians (0 = horizontal) */
} RenderBox;

/* Position of the grid on the original image. */
typedef struct {
    double origin_x;   /* X pixel of grid's top-left corner */
    double origin_y;   /* Y pixel of grid's top-left corner */
    double cell_w;     /* Width of one cell in pixels */
    double cell_h;     /* Height of one cell in pixels */
} GridGeometry;

/* Calculate the pixel box for a found word. */
RenderBox render_calc_word_box(WordPosition pos, GridGeometry geom);

#endif /* RENDER_H */