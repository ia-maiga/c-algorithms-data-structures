/*
 * Part 2ter — FIFO queue implemented as a circular linked list
 *             (constant-time enqueue and dequeue).
 *
 * Authors : Walid Bouzid & Ibrahim Aboubakarine Maiga
 * Context : Algorithms course project, Université Paris-Saclay (2025-2026)
 *
 * Function names follow the original assignment (in French).
 */

#include <stdio.h>
#include <stdlib.h>

/*************************************************/
/*                                               */
/*                type booléen                   */
/*                                               */
/*************************************************/

typedef enum {false, true} bool;

/*************************************************/
/*                                               */
/*          definition type liste                */
/*                                               */
/*************************************************/

typedef struct Bloc {
    int valeur ;
    struct Bloc *suite;
} Bloc;

typedef Bloc* file;

// La file est représentée par un pointeur sur son DERNIER bloc ;
// le dernier bloc pointe sur le premier (liste circulaire).
// Ainsi entree et sortie sont en temps constant.

bool sortie(int *x, file *F);

/********************************************/
/*            longueur                      */
/********************************************/

int longueur_iter (file *F)
{
    if (*F == NULL) return 0;
    Bloc *dernier = *F;
    Bloc *p = dernier->suite; 
    int cpt = 0;
    do {
        cpt++;
        p = p->suite;
    } while (p != dernier->suite);
    return cpt;
}

/********************************************/
/*            vide la file                  */
/********************************************/

void vide_file(file *F)
{
    int x;
    while (sortie(&x, F)) {}
}

/********************************************/
/*            entree : enfile               */
/********************************************/

void entree(int x, file *F)
{
    Bloc* nouv = (Bloc*) malloc(sizeof(Bloc));
    nouv->valeur = x;


    if (*F == NULL) {
        nouv->suite = nouv;
        *F = nouv;
    }
    else {
        nouv->suite = (*F)->suite;   
        (*F)->suite = nouv;          
        *F = nouv;                   
    }
}

/********************************************/
/*            sortie : defile               */
/********************************************/

bool sortie(int *x, file *F)
{
    if (*F == NULL)
        return false;   
    Bloc* dernier = *F;
    Bloc* premier = dernier->suite;
    *x = premier->valeur;
    if (premier == dernier) {
        free(premier);
        *F = NULL;
    }
    else {
        dernier->suite = premier->suite;
        free(premier);
    }
    return true;
}

/********************************************/
/*              affiche_file                */
/********************************************/

void affiche_file(file *F) {
    if (*F == NULL) {
        printf("[vide]\n");
        return;
    }
    Bloc *dernier = *F;
    Bloc *p = dernier->suite; 
    printf("[ ");
    do {
        printf("%d ", p->valeur);
        p = p->suite;
    } while (p != dernier->suite);
    printf("]\n");
}

/********************************************/
/*                 MAIN                     */
/********************************************/

int main() {
    file F = NULL;
    int x;

    printf("File vide : "); affiche_file(&F);

    entree(1, &F); entree(2, &F); entree(3, &F);
    printf("Après entree 1, 2, 3 : "); affiche_file(&F);
    printf("Longueur : %d\n", longueur_iter(&F));

    sortie(&x, &F);
    printf("sortie -> %d, file : ", x); affiche_file(&F);

    entree(4, &F);
    printf("Après entree 4 : "); affiche_file(&F);

    while (sortie(&x, &F))
        printf("sortie -> %d\n", x);
    printf("File finale : "); affiche_file(&F);

    vide_file(&F);
    return 0;
}
