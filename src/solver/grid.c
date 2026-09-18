#include "grid.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Grid *grid_create(int rows, int cols)
{
    if (rows <= 0 || cols <= 0) {
        return NULL;
    }

    Grid *g = malloc(sizeof(*g));
    if (g == NULL) {
        return NULL;
    }

    g->rows = rows;
    g->cols = cols;
    g->cells = malloc((size_t)rows * sizeof(*g->cells));
    if (g->cells == NULL) {
        free(g);
        return NULL;
    }

    for (int r = 0; r < rows; r++) {
        g->cells[r] = calloc((size_t)cols, sizeof(*g->cells[r]));
        if (g->cells[r] == NULL) {
            for (int k = 0; k < r; k++) {
                free(g->cells[k]);
            }
            free(g->cells);
            free(g);
            return NULL;
        }
    }
    return g;
}

void grid_free(Grid *g)
{
    if (g == NULL) {
        return;
    }
    if (g->cells != NULL) {
        for (int r = 0; r < g->rows; r++) {
            free(g->cells[r]);
        }
        free(g->cells);
    }
    free(g);
}

char grid_get(const Grid *g, int row, int col)
{
    if (g == NULL || row < 0 || row >= g->rows
        || col < 0 || col >= g->cols) {
        return '\0';
    }
    return g->cells[row][col];
}

void grid_set(Grid *g, int row, int col, char c)
{
    if (g == NULL || row < 0 || row >= g->rows
        || col < 0 || col >= g->cols) {
        return;
    }
    g->cells[row][col] = c;
}

Grid *grid_load(const char *path)
{
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return NULL;
    }

    int rows = 0;
    int cols = 0;
    if (fscanf(f, "%d %d", &rows, &cols) != 2) {
        fclose(f);
        return NULL;
    }

    Grid *g = grid_create(rows, cols);
    if (g == NULL) {
        fclose(f);
        return NULL;
    }

    char buf[1024];
    for (int r = 0; r < rows; r++) {
        if (fscanf(f, "%1023s", buf) != 1) {
            grid_free(g);
            fclose(f);
            return NULL;
        }
        int len = (int)strlen(buf);
        for (int c = 0; c < cols && c < len; c++) {
            g->cells[r][c] = buf[c];
        }
    }
    fclose(f);
    return g;
}


void grid_print(const Grid *g)
{
    if (g == NULL) {
        return;
    }
    for (int r = 0; r < g->rows; r++) {
        for (int c = 0; c < g->cols; c++) {
            putchar(g->cells[r][c]);
            if (c + 1 < g->cols) {
                putchar(' ');
            }
        }
        putchar('\n');
    }
}
