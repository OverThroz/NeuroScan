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

    char line[1024];
    int rows = 0;
    int cols = 0;
    char **temp_cells = NULL;

    /* Read the first line */
    if (fgets(line, sizeof(line), f) == NULL) {
        fclose(f);
        return NULL;
    }
    
    /* Remove newline characters */
    line[strcspn(line, "\r\n")] = '\0';

    /* Check if the first line contains dimensions (e.g., "5 5") */
    int r, c;
    if (sscanf(line, "%d %d", &r, &c) == 2) {
        rows = r;
        cols = c;
        temp_cells = malloc(rows * sizeof(char*));
        for (int i = 0; i < rows; i++) {
            temp_cells[i] = calloc(cols + 1, sizeof(char));
            if (fgets(line, sizeof(line), f)) {
                line[strcspn(line, "\r\n")] = '\0';
                for (int j = 0; j < cols && j < (int)strlen(line); j++) {
                    temp_cells[i][j] = line[j];
                }
            }
        }
    } else {
        /* No header: the first line is the first row of the grid */
        cols = strlen(line);
        temp_cells = malloc(sizeof(char*));
        temp_cells[0] = calloc(cols + 1, sizeof(char));
        for (int j = 0; j < cols; j++) {
            temp_cells[0][j] = line[j];
        }
        rows = 1;

        while (fgets(line, sizeof(line), f)) {
            line[strcspn(line, "\r\n")] = '\0';
            if (strlen(line) == 0) continue; /* Skip empty lines */
            
            temp_cells = realloc(temp_cells, (rows + 1) * sizeof(char*));
            temp_cells[rows] = calloc(cols + 1, sizeof(char));
            for (int j = 0; j < cols && j < (int)strlen(line); j++) {
                temp_cells[rows][j] = line[j];
            }
            rows++;
        }
    }
    fclose(f);

    if (rows == 0 || cols == 0) {
        free(temp_cells);
        return NULL;
    }

    /* Build the final Grid */
    Grid *g = grid_create(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            g->cells[i][j] = temp_cells[i][j];
        }
        free(temp_cells[i]);
    }
    free(temp_cells);

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
