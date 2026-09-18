#ifndef GRID_H
#define GRID_H

/*
 * A rectangular grid of uppercase letters.
 * cells is stored row-major: cells[row][col].
 */
typedef struct {
    int   rows;
    int   cols;
    char **cells;
} Grid;


/* Allocate an empty grid (cells filled with '\0'). NULL on failure. */
Grid *grid_create(int rows, int cols);

/* Free the grid and its contents. Safe on NULL. */
void  grid_free(Grid *g);

/* Return cells[row][col], or '\0' if out of bounds. */
char  grid_get(const Grid *g, int row, int col);

/* Write c at (row, col) if in bounds; otherwise do nothing. */
void  grid_set(Grid *g, int row, int col, char c);

/* Load a grid from a text file. Returns NULL on failure.
 * Format: first line "rows cols", then rows lines of cols letters.
 */
Grid *grid_load(const char *path);

/* Print the grid to stdout (debug). */
void  grid_print(const Grid *g);

#endif /* GRID_H */
