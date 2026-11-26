#include "render.h"
#include "SDL_rect.h"
#include "SDL_surface.h"
#include <math.h>

/* M_PI is not part of ISO C: define it when -std=c11 hides it. */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

BBox calc_word_box(WordPosition pos, GridGeom geom)
{
    BBox box = {0, 0, 0, 0, 0};

    /* Calculate pixel coordinates of the start and end cells (centers) */
    double x0 = geom.origin_x + (pos.x0 + 0.5) * geom.cell_w;
    double y0 = geom.origin_y + (pos.y0 + 0.5) * geom.cell_h;
    double x1 = geom.origin_x + (pos.x1 + 0.5) * geom.cell_w;
    double y1 = geom.origin_y + (pos.y1 + 0.5) * geom.cell_h;

    /* Distance between start and end */
    double dx = x1 - x0;
    double dy = y1 - y0;
    double length = sqrt(dx * dx + dy * dy);

    /* Box center */
    double cx = (x0 + x1) / 2.0;
    double cy = (y0 + y1) / 2.0;

    /* Box size (add one cell length for padding around the word) */
    box.w = length + geom.cell_w;
    box.h = geom.cell_h * 0.8; /* Make it look like an ellipse */

    /* Rotation angle */
    box.angle = atan2(dy, dx);

    /* Top-left corner (before rotation) */
    box.x = cx - (box.w / 2.0);
    box.y = cy - (box.h / 2.0);

    return box;
}

void draw_word_ellipse(SDL_Surface *image, BBox box, Uint32 color)
{
    double cx;
    double cy;
    double a;
    double b;
    double cos_a;
    double sin_a;
    int steps;

    if (image == NULL)
    {
        return;
    }

    /* Center of the ellipse: middle of the word */
    cx = box.x + box.w/2.0;
    cy = box.y + box.h/2.0;

    /* Semi-axes: half the length and half the height of the box */
    a = box.w / 2.0;
    b = box.h / 2.0;

    /* Rotation of the word, computed once for every point */
    cos_a = cos(box.angle);
    sin_a = sin(box.angle);
    double rad = a;
    if (b > a)
    {
        rad = b;
    }

    /* Two points per pixel of perimeter, so the line has no gaps */
    double double_steps = (4.0 * M_PI * rad) + 1;
    steps = double_steps;

    for (int i = 0; i < steps; i++)
    {
        /* Point of a horizontal ellipse centered on (0, 0) */
        double t = 2.0 *M_PI * i / steps;
        double px = a * cos(t);
        double py = b * sin(t);
        SDL_Rect dot;

        /* Rotate the point by the word angle, then move it to the center
        The 3x3 square is shifted by 1 to be centered on the point. */
        double double_dot_x = (cx + px * cos_a - py * sin_a) - 1;
        double double_dot_y = (cy + px * sin_a + py * cos_a) - 1;
        dot.x = double_dot_x;
        dot.y = double_dot_y;
        dot.w = 3;
        dot.h = 3;

        /* Draw the square; parts outside the image are clipped by SDL */
        SDL_FillRect(image, &dot, color);
    }
}
