#include "render.h"
#include <math.h>

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
