#include "reconstruction.h"
#include <stdio.h>

int main(void)
{
    /* Matrix with OCR mistakes: '0' instead of 'O', '1' instead of 'I' */
    char *matrix[3] = { "CAT", "0OG", "123" };
    Grid *g = reconstruction_build_grid(matrix, 3, 3);
    
    printf("Before corrections:\n");
    grid_print(g);
    
    int changes = reconstruction_apply_corrections(g);
    printf("Corrections applied: %d\n", changes);
    printf("After corrections:\n");
    grid_print(g);
    
    char *words[2] = { "CAT", "DOG" };
    WordList *wl = reconstruction_build_wordlist(words, 2);
    printf("WordList: %s, %s\n", wl->words[0], wl->words[1]);

    grid_free(g);
    wordlist_free(wl);
    return 0;
}