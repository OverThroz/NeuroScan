/*
** EPITA - SAE S3 - 2026
** Project NeuroScan
** Author : Maxime
** Role   : Structure to store the list of words to search
*/

#ifndef WORDLIST_H
#define WORDLIST_H

/*
** Dynamic word list.
** We keep track of the capacity to avoid realloc on every add.
** count = current number of words, capacity = allocated size.
*/
typedef struct {
    int    count;
    int    capacity;
    char **words; // Array of strings (char*)
} WordList;

/*
** Constructor: creates an empty list. If capacity <= 0, defaults to 1.
** Destructor: frees each word string then the array.
*/
WordList *wordlist_create(int capacity);
void      wordlist_free(WordList *wl);

/*
** Adds a word. Performs a deep copy (strdup) so the caller
** does not have to worry about the lifetime of the passed string.
** Returns 0 on success, -1 on allocation error.
** Handles automatic expansion (realloc) if the list is full.
*/
int       wordlist_add(WordList *wl, const char *word);

/* Retrieves a word by its index. Returns NULL if index is invalid. */
const char *wordlist_get(const WordList *wl, int index);

#endif // WORDLIST_H
