#include "reconstruction.h"
#include <stdlib.h>

Grid *recon_grid(char **matrix, int rows, int cols)
{
    if (matrix == NULL || rows <= 0 || cols <= 0) {
        return NULL;
    }

    Grid *g = grid_create(rows, cols);
    if (g == NULL) {
        return NULL;
    }

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            grid_set(g, r, c, matrix[r][c]);
        }
    }
    return g;
}

WordList *recon_wordlist(char **strings, int n)
{
    if (strings == NULL || n <= 0) {
        return NULL;
    }

    WordList *wl = wordlist_create(n);
    if (wl == NULL) {
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        if (wordlist_add(wl, strings[i]) != 0) {
            wordlist_free(wl);
            return NULL;
        }
    }
    return wl;
}

int recon_fix_ocr(Grid *g)
{
    if (g == NULL) {
        return 0;
    }

    int changes = 0;
    for (int r = 0; r < g->rows; r++) {
        for (int c = 0; c < g->cols; c++) {
            char ch = g->cells[r][c];
            char fixed = ch;
            /* Common OCR confusions */
            if (ch == '0') fixed = 'O';
            else if (ch == '1') fixed = 'I';
            else if (ch == '5') fixed = 'S';
            else if (ch == '8') fixed = 'B';
            else if (ch == '6') fixed = 'G';
            else if (ch == '2') fixed = 'Z';
            
            if (fixed != ch) {
                g->cells[r][c] = fixed;
                changes++;
            }
        }
    }
    return changes;
}