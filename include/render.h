/*
** EPITA - SAE S3 - 2026
** Project NeuroScan
** Author : Maxime
** Role   : Bridge between Solver and GUI (Rendering calculation)
*/

#ifndef RENDER_H
#define RENDER_H

#include "wordposition.h"

/*
** Bounding Box used by the GUI (Louis) to draw ellipses.
** We use doubles for precision.
** Angle is in radians: 0 for horizontal, PI/4 for diagonal, etc.
*/
typedef struct {
    double x, y;       // Top-left corner
    double w, h;       // Box width and height
    double angle;      // Rotation in radians
} BBox;

/*
** Represents the grid position in the source image.
** The OCR must provide this info (where the grid starts, cell size).
*/
typedef struct {
    double origin_x;   // X coordinate of the grid's top-left corner
    double origin_y;   // Y coordinate of the grid's top-left corner
    double cell_w;     // Width of a cell in pixels
    double cell_h;     // Height of a cell in pixels
} GridGeom;

/*
** Calculates the pixel bounding box for a found word.
** Computes the word's center, distance between letters (for width),
** and uses atan2() to determine the ellipse rotation angle.
** This allows Louis to draw an oval even for diagonal words.
*/
BBox calc_word_box(WordPosition pos, GridGeom geom);

#endif // RENDER_H
