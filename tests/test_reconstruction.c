#include "reconstruction.h"
#include <stdio.h>

int main(void)
{
    /* Matrix with OCR mistakes: '0' instead of 'O', '1' instead of 'I' */
    char *matrix[3] = { "CAT", "0OG", "123" };
    Grid *g = recon_grid(matrix, 3, 3);
    
    printf("Before corrections:\n");
    grid_print(g);
    
    int changes = recon_fix_ocr(g);
    printf("Corrections applied: %d\n", changes);
    printf("After corrections:\n");
    grid_print(g);
    
    char *words[2] = { "CAT", "DOG" };
    WordList *wl = recon_wordlist(words, 2);
    printf("WordList: %s, %s\n", wl->words[0], wl->words[1]);

    grid_free(g);
    wordlist_free(wl);
    return 0;
}