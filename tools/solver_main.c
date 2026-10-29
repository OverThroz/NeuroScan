#include "grid.h"
#include "wordlist.h"
#include "solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(const char *prog)
{
    fprintf(stderr, "Usage: %s <grid_file> <word> [-q]\n", prog);
    fprintf(stderr, "       %s <grid_file_with_words> [-q]\n", prog);
}

/* Load the list of words from the same file, after the grid. */
static WordList *load_words_after_grid(const char *path, int rows)
{
    FILE *f = fopen(path, "r");
    if (f == NULL) return NULL;

    int r, c;
    if (fscanf(f, "%d %d", &r, &c) != 2) { fclose(f); return NULL; }

    char buf[1024];
    for (int i = 0; i < rows; i++) {
        if (fscanf(f, "%1023s", buf) != 1) { fclose(f); return NULL; }
    }

    int n = 0;
    if (fscanf(f, "%d", &n) != 1 || n < 0) { fclose(f); return NULL; }

    WordList *wl = wordlist_create(n);
    if (wl == NULL) { fclose(f); return NULL; }

    for (int i = 0; i < n; i++) {
        if (fscanf(f, "%1023s", buf) != 1) {
            wordlist_free(wl); fclose(f); return NULL;
        }
        if (wordlist_add(wl, buf) != 0) {
            wordlist_free(wl); fclose(f); return NULL;
        }
    }
    fclose(f);
    return wl;
}

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 4) {
        usage(argv[0]);
        return 1;
    }

    int quiet = 0;
    /* Check for -q flag at the end */
    if (strcmp(argv[argc - 1], "-q") == 0) {
        quiet = 1;
        argc--;
    }

    Grid *g = grid_load(argv[1]);
    if (g == NULL) {
        fprintf(stderr, "Error: cannot load grid from '%s'\n", argv[1]);
        return 1;
    }

    WordList *wl = NULL;
    if (argc == 3) {
        /* Format imposé par le sujet : ./solver grille mot */
        quiet = 1; /* En mode 1 mot, on force l'affichage strict (x0,y0)(x1,y1) */
        wl = wordlist_create(1);
        if (wl) wordlist_add(wl, argv[2]);
    } else if (argc == 2) {
        /* Ancien format pour les tests : lit la liste dans le fichier */
        wl = load_words_after_grid(argv[1], g->rows);
    } else {
        usage(argv[0]);
        grid_free(g);
        return 1;
    }

    if (wl == NULL) {
        fprintf(stderr, "Error: cannot load word list\n");
        grid_free(g);
        return 1;
    }

    WordResult *results = solver_solve_all(g, wl);
    if (results == NULL) {
        fprintf(stderr, "Error: solver failed\n");
        wordlist_free(wl);
        grid_free(g);
        return 1;
    }

    for (int i = 0; i < wl->count; i++) {
        solver_print_result(&results[i], quiet);
    }

    free(results);
    wordlist_free(wl);
    grid_free(g);
    return 0;
}