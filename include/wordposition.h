#ifndef WORDPOSITION_H
#define WORDPOSITION_H

/*
 * Result of a search for a word in the grid.
 * Coordinates are (x = column, y = row), 0-indexed.
 * found == 0 means "Not found".
 */
typedef struct {
    int x0, y0;
    int x1, y1;
    int found;
} WordPosition;

/* A word paired with its position in the grid. */
typedef struct {
    const char   *word;
    WordPosition  pos;
} WordResult;

#endif /* WORDPOSITION_H */
