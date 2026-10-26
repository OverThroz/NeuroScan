#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include "grid.h"
#include "wordlist.h"

/*
 * Build a Grid from a 2D matrix of characters (OCR output).
 * matrix: array of `rows` strings, each of length `cols`.
 * Returns a new Grid, or NULL on failure.
 */
Grid *reconstruction_build_grid(char **matrix, int rows, int cols);

/*
 * Build a WordList from an array of strings.
 * Returns a new WordList, or NULL on failure.
 */
WordList *reconstruction_build_wordlist(char **strings, int n);

/*
 * Fix common OCR mistakes in the grid (Bonus).
 * E.g., replaces '0' with 'O', '1' with 'I', etc.
 * Returns the number of cells modified.
 */
int reconstruction_apply_corrections(Grid *g);

#endif /* RECONSTRUCTION_H */