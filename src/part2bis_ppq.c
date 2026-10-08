/*
 * Part 2bis — PPQ: all lists of integers in [p1, p2] summing to q,
 *             generated in lexicographic order (lists of lists).
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

typedef struct Bloc
{
    int valeur ;
    struct Bloc * suite;
} Bloc;

typedef Bloc *Liste ;

typedef struct BlocDeBloc
{
    Liste valeur ;
    struct BlocDeBloc * suite;
} BlocDeBloc;
typedef BlocDeBloc *ListeDeListe ;

/*************************************************/
/*                                               */
/*                briques de base                */
/*                                               */
/*************************************************/


/****************/

void depile(Liste *L)
{   Liste tmp = *L ;
    *L = (*L)->suite ;
    free(tmp) ;
}

/*******/

Liste ajoute(int x, Liste l)
{   Liste tmp = (Liste) malloc(sizeof(Bloc)) ;
    tmp->valeur = x ;
    tmp->suite = l ;
    return tmp ;
}

ListeDeListe ajouteLDL(Liste x, ListeDeListe l)
{   ListeDeListe tmp = (ListeDeListe) malloc(sizeof(BlocDeBloc)) ;
    tmp->valeur = x ;
    tmp->suite = l ;
    return tmp ;
}

/*******/

void empile(int x, Liste *L) 
{ *L = ajoute(x,*L) ; }

/*****************************/
/*                           */
/*       Affiche             */
/*                           */
/*****************************/

void affiche_rec(Liste l)
{
    if (l == NULL)
        printf("\n");
    else
    {
        printf("%d ", l->valeur);
        affiche_rec(l->suite);
    }
}


/*******/

void affiche_iter(Liste l)
{
    Liste L2 = l;
    while( L2 != NULL )
    {
        printf("%d ", L2->valeur);
        L2 = L2->suite;
    }
    printf("\n");
}

// Affiche une liste de listes sous la forme [[2 2 3] [3 4] ...]
void afficheLDL_iter(ListeDeListe l)
{
    printf("[");
    for (ListeDeListe L2 = l; L2 != NULL; L2 = L2->suite)
    {
        printf("[");
        for (Liste L3 = L2->valeur; L3 != NULL; L3 = L3->suite)
            printf(L3->suite != NULL ? "%d " : "%d", L3->valeur);
        printf(L2->suite != NULL ? "] " : "]");
    }
    printf("]\n");
}

/****************************/
/*                          */
/*       Longueur           */
/*                          */
/****************************/

int longueur_rec (Liste l)
{
    if (l == NULL)
         return 0 ;
    else return (1 + longueur_rec(l->suite)) ;
}

/*******/

int longueur_iter (Liste l)
{
    Liste P = l;
    int cpt = 0 ;
    while (P != NULL)
    {   P = P->suite ;
        cpt++ ;
    }
    return cpt ;
}

/*****************************************/
/*                                       */
/*                 VireDernier           */
/*     avec un depile                    */
/* à la main opportuniste (version iter) */
/* ou en utilisant depile (version rec ) */ 
/*                                       */
/*****************************************/

void VD (Liste *L)
          // *L non NULL ie liste non vide
{
     if ( (*L)->suite == NULL )
            depile(L) ;   // moralement : depile(& (*L)) ;
     else VD (& (*L)->suite) ;
}

void VireDernier_rec (Liste *L)
{
     if ( *L != NULL )
          VD(L);        // moralement : VD(& (*L)) ;
}

/*************/

void VireDernier_iter (Liste *L)
{
    if ( *L != NULL)
    {
        while ( (*L)->suite != NULL )
                 L = & (*L)->suite  ;   //  &  (**L).suite  ;
        free(*L) ;
        *L = NULL ;
     }
}


/*************************************************/
/*                                               */
/*       Libere la memoire                       */
/*                                               */
/*************************************************/

void VideListe(Liste *L)
{
    if ( *L != NULL )
    {
        depile(L);
        VideListe(L);
    }
      
}

/********************************************/
/*                                          */
/*                PPQ                       */
/*                                          */
/********************************************/

// AETTDL : Ajoute En Tête de Toutes les Listes.
// Version destructive : on modifie les listes de a sur place au lieu
// de les recopier, ce qui évite les fuites mémoire (aucun bloc perdu).

ListeDeListe AETTDL(int p, ListeDeListe a) {
    for (ListeDeListe c = a; c != NULL; c = c->suite)
        c->valeur = ajoute(p, c->valeur);
    return a;
}

// Concat : accroche l2 au bout de l1 (destructive, pas de copie).

ListeDeListe Concat(ListeDeListe l1, ListeDeListe l2) {
    if (l1 == NULL) return l2;
    ListeDeListe c = l1;
    while (c->suite != NULL)
        c = c->suite;
    c->suite = l2;
    return l1;
}

// Libère une liste de listes et toutes ses sous-listes.

void VideLDL(ListeDeListe *L) {
    while (*L != NULL) {
        ListeDeListe tmp = *L;
        VideListe(&tmp->valeur);
        *L = tmp->suite;
        free(tmp);
    }
}

/*
bool test_si_singleton(int a, int b, int c) {
    // Fonction écrite pour tester si un cas PPQ est un singleton et c doit etre
    // compris entre a et b
    if(c<a || c > b) {
        return false;
    }
    for(int i = a; i<=b; i++) {
        for(int j = i; j<=b; j++) {
            if(i+j == c) {
                return false;
            }
        }
    }
    return true;
}
*/

bool test_si_singleton(int a, int b, int c) {
    // Fonction écrite pour tester si un cas PPQ est un singleton et c doit etre
    // compris entre a et b
    /*if(c<a || c > b) {
        return false;
    } */
    if(b==a) {
        if(a+b == c) {
            return false;
        }
        else {
            return true;
        }
    }
    return !(a+b == c) && test_si_singleton(a, b-1, c);
}


ListeDeListe PPQ(int p1, int p2, int q) {
    ListeDeListe res = NULL;
    if (q==0) {
        Liste l1 = NULL;
        res = ajouteLDL(l1, res);
        return res;
    }
    if(q<p2 && q>p1) {
    if(test_si_singleton(p1, p2, q) == true) {
        Liste l1 = NULL;
        l1 = ajoute(q, l1);
        res = ajouteLDL(l1, res);
        return res;
    }
    }
    if (p1>q) {
        return res;
    }
    else {
        int i = p2;
        while (i>=p1) {
            res = Concat( AETTDL(i, PPQ(p1, p2, q-i)), res);
            i = i-1;
        }
        return res;
    }
}


/*************************************************/
/*                                               */
/*           Main : exemples de l'énoncé         */
/*                                               */
/*************************************************/

int main() {
    ListeDeListe r;

    printf("PPQ(2, 4, 9) :\n");
    r = PPQ(2, 4, 9); afficheLDL_iter(r); VideLDL(&r);

    printf("PPQ(2, 4, 0) (attendu [[]], une liste vide) :\n");
    r = PPQ(2, 4, 0); afficheLDL_iter(r); VideLDL(&r);

    printf("PPQ(2, 4, 1) (attendu [], aucune liste) :\n");
    r = PPQ(2, 4, 1); afficheLDL_iter(r); VideLDL(&r);

    printf("PPQ(1, 3, 5) :\n");
    r = PPQ(1, 3, 5); afficheLDL_iter(r); VideLDL(&r);

    return 0;
}
