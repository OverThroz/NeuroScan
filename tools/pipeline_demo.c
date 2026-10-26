#include "reconstruction.h"
#include "solver.h"
#include "render.h"

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    /* 
     * 1. SIMULATION DE L'OCR (Ibrahim)
     * On imagine que l'OCR a mal lu les lettres et a mis des '0' et '1'.
     */
    char *ocr_matrix[3] = {
        "CAT", /* Correct */
        "0OG", /* Erreur : '0' au lieu de 'O' */
        "1IS"  /* Erreur : '1' au lieu de 'I' */
    };
    int rows = 3;
    int cols = 3;

    /* 2. RECONSTRUCTION ET CORRECTION (Toi) */
    Grid *g = reconstruction_build_grid(ocr_matrix, rows, cols);
    printf("--- 1. Sortie brute de l'OCR ---\n");
    grid_print(g);

    int fixes = reconstruction_apply_corrections(g);
    printf("\n--- 2. Grille apres correction (%d erreurs corrigees) ---\n", fixes);
    grid_print(g);

    /* Liste de mots à chercher (simulant l'OCR de la liste de mots) */
    char *words[2] = { "CAT", "DOG" };
    WordList *wl = reconstruction_build_wordlist(words, 2);

    /* 3. RESOLUTION (Ton solver) */
    WordResult *results = solver_solve_all(g, wl);

    /* 4. CALCUL DES COORDONNEES (Toi, pour Louis) */
    /* On imagine que la grille commence au pixel (100, 100) et que les cases font 50x50 px */
    GridGeometry geom = { 100.0, 100.0, 50.0, 50.0 };

    printf("\n--- 3. Resultats du Solver et Coordonnees de Rendu ---\n");
    for (int i = 0; i < wl->count; i++) {
        if (results[i].pos.found) {
            RenderBox box = render_calc_word_box(results[i].pos, geom);
            printf("Mot '%s' trouve !\n", results[i].word);
            printf("  -> Grille : (x0=%d, y0=%d) a (x1=%d, y1=%d)\n",
                   results[i].pos.x0, results[i].pos.y0,
                   results[i].pos.x1, results[i].pos.y1);
            printf("  -> Pixels : Dessiner une ellipse a x=%.1f, y=%.1f, largeur=%.1f, hauteur=%.1f, angle=%.2f rad\n",
                   box.x, box.y, box.w, box.h, box.angle);
        } else {
            printf("Mot '%s' non trouve.\n", results[i].word);
        }
    }

    /* 5. NETTOYAGE MEMOIRE */
    free(results);
    wordlist_free(wl);
    grid_free(g);

    return 0;
}