/*
** EPITA - SAE S3 - 2026
** Project NeuroScan
** Author : Maxime
** Role   : Core data structures (the Grid)
*/

#ifndef GRID_H
#define GRID_H

/*
** Structure representing the word search grid.
** Uses a dynamically allocated 2D array.
** Note: the grid is indexed as [row][col].
*/
typedef struct {
    int   rows;
    int   cols;
    char **cells; // Char matrix (e.g., cells[r][c] = 'A')
} Grid;

/*
** Constructor / Destructor
** grid_create : allocates the grid and initializes all cells to '\0'.
**               Returns NULL if dimensions are invalid (<=0) or if
**               memory allocation fails.
** grid_free   : frees each row and then the main array. Safe on NULL.
*/
Grid *grid_create(int rows, int cols);
void  grid_free(Grid *g);

/*
** Safe accessors.
** Unlike direct access, these functions check the bounds.
** grid_get : returns '\0' if out of bounds (prevents segfaults).
** grid_set : writes the char only if within bounds.
*/
char  grid_get(const Grid *g, int row, int col);
void  grid_set(Grid *g, int row, int col, char c);

/*
** File parsing (used by the CLI module).
** Expected format: first line "rows cols", then lines of joined letters.
** Returns NULL if the file is not found or malformed.
*/
Grid *grid_load(const char *path);

/* Debug print to terminal */
void  grid_print(const Grid *g);

#endif // GRID_H
