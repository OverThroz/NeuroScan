/*
** EPITA - SAE S3 - 2026
** Projet NeuroScan
** Auteur : Maxime
** Role   : Structure pour stocker la liste des mots à chercher
*/

#ifndef WORDLIST_H
#define WORDLIST_H

/*
** Liste de mots dynamique.
** On garde une trace de la capacite pour éviter de realloc à chaque ajout.
** count = nombre de mots actuel, capacity = taille allouée.
*/
typedef struct {
    int    count;
    int    capacity;
    char **words; // Tableau de chaînes de caractères (char*)
} WordList;

/*
** Constructeur : crée une liste vide. Si capacity <= 0, on met 1 par défaut.
** Destructeur : libère chaque mot (strdup) puis le tableau.
*/
WordList *wordlist_create(int capacity);
void      wordlist_free(WordList *wl);

/*
** Ajoute un mot. On fait une copie profonde (strdup) pour que l'appelant 
** n'ait pas à se soucier de la durée de vie de la chaîne passée en param.
** Retourne 0 si succès, -1 si erreur d'allocation.
** Gère l'agrandissement automatique (realloc) si la liste est pleine.
*/
int       wordlist_add(WordList *wl, const char *word);

/* Récupère un mot par son index. Retourne NULL si l'index est invalide. */
const char *wordlist_get(const WordList *wl, int index);

#endif // WORDLIST_H