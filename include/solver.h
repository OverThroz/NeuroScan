/*
** EPITA - SAE S3 - 2026
** Projet NeuroScan
** Auteur : Maxime
** Role   : Moteur de recherche des mots (Algorithme des 8 directions)
*/

#ifndef SOLVER_H
#define SOLVER_H

#include "grid.h"
#include "wordlist.h"
#include "wordposition.h"

/*
** Cherche UN mot dans la grille.
** Algorithme naïf : on parcourt la grille, et dès qu'on trouve la première
** lettre du mot, on lance une vérification dans les 8 directions 
** (Haut, Bas, Gauche, Droite, et les 4 diagonales).
** Complexité : O(N * M * 8 * L) avec L la longueur du mot.
** Retourne une WordPosition avec found=0 si le mot n'est pas là.
*/
WordPosition solver_find_word(const Grid *g, const char *word);

/*
** Résout toute la liste de mots d'un coup.
** Retourne un tableau de WordResult alloué dynamiquement.
** C'est à l'appelant de free() le tableau retourné.
*/
WordResult *solver_solve_all(const Grid *g, const WordList *wl);

/*
** Affiche le résultat formaté pour le CLI.
** Le sujet exige "(x0,y0)(x1,y1)" ou "Not found". 
** Si quiet == 1, on affiche STRICTEMENT ça (mode -q).
** Si quiet == 0, on préfixe avec le mot pour faciliter le debug.
*/
void solver_print_result(const WordResult *r, int quiet);

#endif // SOLVER_H