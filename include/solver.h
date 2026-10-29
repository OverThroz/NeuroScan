/*
** EPITA - SAE S3 - 2026
** Project NeuroScan
** Author : Maxime
** Role   : Word search engine (8-direction algorithm)
*/

#ifndef SOLVER_H
#define SOLVER_H

#include "grid.h"
#include "wordlist.h"
#include "wordposition.h"

/*
** Searches for ONE word in the grid.
** Naive algorithm: iterate through the grid, and as soon as we find the
** first letter of the word, check all 8 directions
** (Up, Down, Left, Right, and the 4 diagonals).
** Complexity: O(N * M * 8 * L) where L is the length of the word.
** Returns a WordPosition with found=0 if the word is not present.
*/
WordPosition solver_find_word(const Grid *g, const char *word);

/*
** Solves the entire word list at once.
** Returns a dynamically allocated array of WordResult.
** The caller is responsible for freeing the returned array.
*/
WordResult *solver_solve_all(const Grid *g, const WordList *wl);

/*
** Prints the formatted result for the CLI.
** The spec requires "(x0,y0)(x1,y1)" or "Not found".
** If quiet == 1, print STRICTLY this format (-q mode).
** If quiet == 0, prefix with the word to ease debugging.
*/
void solver_print_result(const WordResult *r, int quiet);

#endif // SOLVER_H