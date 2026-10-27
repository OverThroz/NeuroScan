/*
** EPITA - SAE S3 - 2026
** Projet NeuroScan
** Auteur : Maxime
** Role   : Pont entre l'OCR et le Solver (Reconstruction de la grille)
*/

#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include "grid.h"
#include "wordlist.h"

/*
** Prend la matrice de caractères brute sortie par l'OCR (Ibrahim) et la 
** transforme en structure Grid propre à notre projet.
** matrix : tableau de chaînes (ex: char *matrix[] = {"CAT", "DOG"}).
*/
Grid *recon_grid(char **matrix, int rows, int cols);

/*
** Fait la même chose mais pour la liste de mots.
** Prend un tableau de chaînes de caractères et construit la WordList.
*/
WordList *recon_wordlist(char **strings, int n);

/*
** BONUS / ROBUSTESSE : Correction des erreurs d'OCR.
** Le réseau de neurones confond souvent les lettres et les chiffres.
** Cette fonction parcourt la grille et remplace les erreurs classiques :
** '0' -> 'O', '1' -> 'I', '5' -> 'S', '8' -> 'B', etc.
** Retourne le nombre de cellules qui ont été modifiées.
*/
int recon_fix_ocr(Grid *g);

#endif // RECONSTRUCTION_H