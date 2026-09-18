#include "wordlist.h"

#include <stdlib.h>
#include <string.h>

WordList *wordlist_create(int capacity)
{
    if (capacity < 0) {
        return NULL;
    }

    WordList *wl = malloc(sizeof(*wl));
    if (wl == NULL) {
        return NULL;
    }

    wl->count = 0;
    wl->capacity = capacity > 0 ? capacity : 1;
    wl->words = malloc((size_t)wl->capacity * sizeof(*wl->words));
    if (wl->words == NULL) {
        free(wl);
        return NULL;
    }
    return wl;
}

static int wordlist_grow(WordList *wl)
{
    int new_cap = wl->capacity * 2;
    char **tmp = realloc(wl->words,
                         (size_t)new_cap * sizeof(*wl->words));
    if (tmp == NULL) {
        return -1;
    }
    wl->words = tmp;
    wl->capacity = new_cap;
    return 0;
}

int wordlist_add(WordList *wl, const char *word)
{
    if (wl == NULL || word == NULL) {
        return -1;
    }
    if (wl->count >= wl->capacity && wordlist_grow(wl) != 0) {
        return -1;
    }

    size_t len = strlen(word);
    char *copy = malloc(len + 1);
    if (copy == NULL) {
        return -1;
    }
    memcpy(copy, word, len + 1);
    wl->words[wl->count++] = copy;
    return 0;
}

void wordlist_free(WordList *wl)
{
    if (wl == NULL) {
        return;
    }
    for (int i = 0; i < wl->count; i++) {
        free(wl->words[i]);
    }
    free(wl->words);
    free(wl);
}

const char *wordlist_get(const WordList *wl, int index)
{
    if (wl == NULL || index < 0 || index >= wl->count) {
        return NULL;
    }
    return wl->words[index];
}
