/*
** EPITA - SAE S3 - 2026
** Project NeuroScan
** Author : Maxime
** Role   : Structures for search results
*/

#ifndef WORDPOSITION_H
#define WORDPOSITION_H

/*
** Represents the coordinates of a found word.
** CRUCIAL CONVENTION: x is the COLUMN, y is the ROW.
** Indexing starts at 0.
** The 'found' flag indicates if the search succeeded (1) or not (0).
** The spec requires a strict output format like "(x0,y0)(x1,y1)".
*/
typedef struct {
    int x0, y0; // Start coordinates
    int x1, y1; // End coordinates
    int found;  // Boolean: 1 if found, 0 otherwise
} WordPosition;

/*
** Associates a word (pointer to the WordList) with its position in the grid.
** Used for final rendering by the GUI.
*/
typedef struct {
    const char   *word;
    WordPosition  pos;
} WordResult;

#endif // WORDPOSITION_H