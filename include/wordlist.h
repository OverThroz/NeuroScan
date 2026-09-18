#ifndef WORDLIST_H
#define WORDLIST_H

typedef struct {
    int    count;
    int    capacity;
    char **words;
} WordList;

/* Create an empty list with the given capacity. */
WordList *wordlist_create(int capacity);

/* Append a copy of word. Returns 0 on success, -1 on failure. */
int       wordlist_add(WordList *wl, const char *word);

/* Free the list and all its words. */
void      wordlist_free(WordList *wl);

/* Return the word at index, or NULL if out of bounds. */
const char *wordlist_get(const WordList *wl, int index);

#endif /* WORDLIST_H */
