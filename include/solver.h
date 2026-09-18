#ifndef SOLVER_H
#define SOLVER_H

#include "grid.h"
#include "wordlist.h"
#include "wordposition.h"

/* Search for a single word in the grid, in any of the 8 directions. */
WordPosition solver_find_word(const Grid *g, const char *word);

/* Resolve every word of the list. Caller owns the returned array
 * (free() it). Returns NULL on allocation failure.
 */
WordResult *solver_solve_all(const Grid *g, const WordList *wl);

/* Print the result of one word.
 * If quiet != 0, prints only "(x0,y0)(x1,y1)" or "Not found".
 * Otherwise prefixes the word.
 */
void solver_print_result(const WordResult *r, int quiet);

#endif /* SOLVER_H */
