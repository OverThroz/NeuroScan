/*
** EPITA - SAE S3 - 2026
** Projet NeuroScan
** Auteur : Maxime
** Role   : Structures pour les résultats de recherche
*/

#ifndef WORDPOSITION_H
#define WORDPOSITION_H

/*
** Représente les coordonnées d'un mot trouvé.
** CONVENTION CRUCIALE : x correspond à la COLONNE, y à la LIGNE. 
** L'indexation commence à 0.
** Le flag 'found' permet de savoir si la recherche a réussi (1) ou non (0).
** Le sujet impose un affichage strict du type "(x0,y0)(x1,y1)".
*/
typedef struct {
    int x0, y0; // Coordonnées de départ
    int x1, y1; // Coordonnées de fin
    int found;  // Booléen : 1 si trouvé, 0 sinon
} WordPosition;

/*
** Associe un mot (pointeur vers la WordList) à sa position dans la grille.
** Utilisé pour le rendu final par l'interface graphique.
*/
typedef struct {
    const char   *word;
    WordPosition  pos;
} WordResult;

#endif // WORDPOSITION_H