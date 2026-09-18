#include "solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 8 directions: E, SE, S, SW, W, NW, N, NE. */
static const int DX[8] = { 1,  1,  0, -1, -1, -1,  0,  1 };
static const int DY[8] = { 0,  1,  1,  1,  0, -1, -1, -1 };

static int match_dir(const Grid *g, const char *word,
                     int row, int col, int d)
{
    int len = (int)strlen(word);
    for (int i = 0; i < len; i++) {
        int r = row + DY[d] * i;
        int c = col + DX[d] * i;
        char gc = grid_get(g, r, c);
        if (gc == '\0' || gc != word[i]) {
            return 0;
        }
    }
    return 1;
}

WordPosition solver_find_word(const Grid *g, const char *word)
{
    WordPosition not_found = { 0, 0, 0, 0, 0 };

    if (g == NULL || word == NULL || word[0] == '\0') {
        return not_found;
    }

    int len = (int)strlen(word);

    for (int r = 0; r < g->rows; r++) {
        for (int c = 0; c < g->cols; c++) {
            if (g->cells[r][c] != word[0]) {
                continue;
            }
            for (int d = 0; d < 8; d++) {
                if (match_dir(g, word, r, c, d)) {
                    WordPosition p;
                    p.x0 = c;
                    p.y0 = r;
                    p.x1 = c + DX[d] * (len - 1);
                    p.y1 = r + DY[d] * (len - 1);
                    p.found = 1;
                    return p;
                }
            }
        }
    }
    return not_found;
}

WordResult *solver_solve_all(const Grid *g, const WordList *wl)
{
    if (g == NULL || wl == NULL || wl->count <= 0) {
        return NULL;
    }

    WordResult *results = malloc((size_t)wl->count * sizeof(*results));
    if (results == NULL) {
        return NULL;
    }

    for (int i = 0; i < wl->count; i++) {
        results[i].word = wl->words[i];
        results[i].pos  = solver_find_word(g, wl->words[i]);
    }
    return results;
}

void solver_print_result(const WordResult *r, int quiet)
{
    if (r == NULL) {
        return;
    }

    if (!quiet && r->word != NULL) {
        printf("%s ", r->word);
    }

    if (r->pos.found) {
        printf("(%d,%d)(%d,%d)\n",
               r->pos.x0, r->pos.y0, r->pos.x1, r->pos.y1);
    } else {
        printf("Not found\n");
    }
}
