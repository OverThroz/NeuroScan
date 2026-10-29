/*
** EPITA - SAE S3 - 2026
** Project NeuroScan
** Author : Maxime
** Role   : End-to-end pipeline demonstration
*/

#include "reconstruction.h"
#include "solver.h"
#include "render.h"

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    /* 
     * 1. OCR SIMULATION (Ibrahim)
     * Assume the OCR misread the letters and put '0' and '1'.
     */
    char *ocr_matrix[3] = {
        "CAT", /* Correct */
        "0OG", /* Error: '0' instead of 'O' */
        "1IS"  /* Error: '1' instead of 'I' */
    };
    int rows = 3;
    int cols = 3;

    /* 2. RECONSTRUCTION AND CORRECTION (Maxime) */
    Grid *g = recon_grid(ocr_matrix, rows, cols);
    printf("--- 1. Raw OCR Output ---\n");
    grid_print(g);

    int fixes = recon_fix_ocr(g);
    printf("\n--- 2. Grid after correction (%d errors fixed) ---\n", fixes);
    grid_print(g);

    /* Word list to search (simulating OCR of the list) */
    char *words[2] = { "CAT", "DOG" };
    WordList *wl = recon_wordlist(words, 2);

    /* 3. SOLVING (Maxime's solver) */
    WordResult *results = solver_solve_all(g, wl);

    /* 4. COORDINATES CALCULATION (Maxime, for Louis) */
    /* Assume grid starts at pixel (100, 100) with 50x50 px cells */
    GridGeom geom = { 100.0, 100.0, 50.0, 50.0 };

    printf("\n--- 3. Solver Results and Render Coordinates ---\n");
    for (int i = 0; i < wl->count; i++) {
        if (results[i].pos.found) {
            BBox box = calc_word_box(results[i].pos, geom);
            printf("Word '%s' found!\n", results[i].word);
            printf("  -> Grid: (x0=%d, y0=%d) to (x1=%d, y1=%d)\n",
                   results[i].pos.x0, results[i].pos.y0,
                   results[i].pos.x1, results[i].pos.y1);
            printf("  -> Pixels: Draw an ellipse at x=%.1f, y=%.1f, "
                   "w=%.1f, h=%.1f, angle=%.2f rad\n",
                   box.x, box.y, box.w, box.h, box.angle);
        } else {
            printf("Word '%s' not found.\n", results[i].word);
        }
    }

    /* 5. MEMORY CLEANUP */
    free(results);
    wordlist_free(wl);
    grid_free(g);

    return 0;
}