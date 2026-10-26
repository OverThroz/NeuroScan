#include "render.h"
#include <stdio.h>

int main(void)
{
    /* 3x3 grid, cell size 100x100, starting at pixel (50, 50) */
    GridGeometry geom = { 50.0, 50.0, 100.0, 100.0 };
    
    /* Word from (0,0) to (2,0) - Horizontal */
    WordPosition pos = { 0, 0, 2, 0, 1 };
    RenderBox box = render_calc_word_box(pos, geom);
    
    printf("Horizontal word box:\n");
    printf("  Center: x=%.1f y=%.1f\n", box.x + box.w/2, box.y + box.h/2);
    printf("  Size:   w=%.1f h=%.1f\n", box.w, box.h);
    printf("  Angle:  %.2f rad (should be 0)\n", box.angle);
    
    return 0;
}