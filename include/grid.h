/*
** EPITA - SAE S3 - 2026
** Projet NeuroScan
** Auteur : Maxime
** Role   : Structures de donnees de base (la Grille)
*/

#ifndef GRID_H
#define GRID_H

/*
** Structure représentant la grille de mots cachés.
** On utilise un tableau 2D alloué dynamiquement.
** Attention : la grille est indexée par [ligne][colonne].
*/
typedef struct {
    int   rows;
    int   cols;
    char **cells; // Matrice de char (ex: cells[r][c] = 'A')
} Grid;

/*
** Constructeur / Destructeur
** grid_create : alloue la grille et initialise toutes les cases à '\0'.
**               Retourne NULL si les dimensions sont invalides (<=0) ou si
**               l'allocation échoue.
** grid_free   : libère chaque ligne puis le tableau principal. Safe sur NULL.
*/
Grid *grid_create(int rows, int cols);
void  grid_free(Grid *g);

/*
** Accesseurs sécurisés.
** Contrairement à un accès direct, ces fonctions vérifient les bornes.
** grid_get : retourne '\0' si on est hors de la grille (évite les segfaults).
** grid_set : écrit le char uniquement si on est dans les bornes.
*/
char  grid_get(const Grid *g, int row, int col);
void  grid_set(Grid *g, int row, int col, char c);

/*
** Parsing de fichier (utilisé par le module CLI).
** Format attendu : 1ère ligne "rows cols", puis les lignes de lettres collées.
** Retourne NULL si le fichier est introuvable ou mal formaté.
*/
Grid *grid_load(const char *path);

/* Affichage debug dans le terminal */
void  grid_print(const Grid *g);

#endif // GRID_H