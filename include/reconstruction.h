/*
** EPITA - SAE S3 - 2026
** Project NeuroScan
** Author : Maxime
** Role   : Bridge between OCR and Solver (Grid reconstruction)
*/

#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include "grid.h"
#include "wordlist.h"

/*
** Takes the raw character matrix output by the OCR (Ibrahim) and
** transforms it into a clean Grid structure for our project.
** matrix: array of strings (e.g., char *matrix[] = {"CAT", "DOG"}).
*/
Grid *recon_grid(char **matrix, int rows, int cols);

/*
** Does the same for the word list.
** Takes an array of strings and builds the WordList.
*/
WordList *recon_wordlist(char **strings, int n);

/*
** BONUS / ROBUSTNESS: OCR error correction.
** The neural network often confuses letters and numbers.
** This function scans the grid and replaces common errors:
** '0' -> 'O', '1' -> 'I', '5' -> 'S', '8' -> 'B', etc.
** Returns the number of cells that were modified.
*/
int recon_fix_ocr(Grid *g);

#endif // RECONSTRUCTION_H
