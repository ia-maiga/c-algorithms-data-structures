/*
 * Part 2 — Singly linked lists: recursive and iterative algorithms,
 *          pointer-to-pointer manipulation.
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
/*          UnPlusDeuxgalTrois              */
/*                                          */
/********************************************/

bool UnPlusDeuxEgalTrois(Liste L) {
    int v0 = 0, v1 = 0, v2 = 0;

    if (L != NULL) {
        v0 = L->valeur;
        if (L->suite != NULL) {
            v1 = L->suite->valeur;
            if (L->suite->suite != NULL) {
                v2 = L->suite->suite->valeur;
            }
        }
    }

    return (v0 + v1 == v2);
}

   
/********************************************/
/*                                          */
/*             PlusCourte                   */
/*                                          */
/********************************************/

bool PlusCourteRec(Liste L1, Liste L2)
{
    if (L1 == NULL && L2 != NULL)
        return true;
    if (L1 != NULL && L2 == NULL)
        return false;
    if (L1 == NULL && L2 == NULL)
        return false;
    return PlusCourteRec(L1->suite, L2->suite);
}


/*******/
  
bool PlusCourteIter(Liste L1, Liste L2)
{
    while (L1 != NULL && L2 != NULL) {
        L1 = L1->suite;
        L2 = L2->suite;
    }
    if (L1 == NULL && L2 != NULL) return true;
    return false;
}

   
  
/********************************************/
/*                                          */
/*              Verifiek0                   */
/*                                          */
/********************************************/

bool VerifiekORec(Liste L, int k)
{
    if (k < 0)                 // trop de zéros déjà vus : inutile d'aller plus loin
        return false;
    if (L == NULL)
        return (k == 0);

    if (L->valeur == 0)
        return VerifiekORec(L->suite, k - 1);

    return VerifiekORec(L->suite, k);
}

   
/*******/

bool VerifiekOIter(Liste L, int k)
{
    while (L != NULL) {
        if (L->valeur == 0)
            k--;

        if (k < 0)
            return false;

        L = L->suite;
    }

    return (k == 0);
}

   

/********************************************/
/*                                          */
/*     NombreTermesAvantZero                */
/*                                          */
/********************************************/

int NTAZ_It(Liste L)
{
    int c = 0;

    while (L != NULL) {
        if (L->valeur == 0)
            return c;

        c++;
        L = L->suite;
    }

    return c;
}


/*******/

int NTAZ_Rec(Liste L)
{
    if (L == NULL) return 0;
    if (L->valeur == 0) return 0;

    return 1 + NTAZ_Rec(L->suite);
}


/*******/

int NTAZ_RTSF_AUX(Liste L, int cpt) {
    if (L == NULL) return cpt;
    if (L->valeur == 0) return cpt;
    return NTAZ_RTSF_AUX(L->suite, cpt + 1);
}

int NTAZ_RTSF(Liste L) {
    return NTAZ_RTSF_AUX(L, 0);
}


/*******/

static void NTAZ_RTSP_AUX(Liste L, int *cpt) {
    if (L == NULL) return;
    if (L->valeur == 0) return;

    (*cpt)++;
    NTAZ_RTSP_AUX(L->suite, cpt);
}

int NTAZ_RTSP(Liste L) {
    int cpt = 0;
    NTAZ_RTSP_AUX(L, &cpt);
    return cpt;
}


/********************************************/
/*                                          */
/*              TuePos                      */
/*                                          */
/********************************************/

// Les positions commencent à 1 et sont celles de la liste d'origine :
// après une suppression, l'élément suivant a donc la position pos+1.

static void TuePosRecAux(Liste *L, int pos) {
    if (*L == NULL) return;

    if ((*L)->valeur == pos) {
        depile(L);                       // *L pointe maintenant sur le suivant
        TuePosRecAux(L, pos + 1);
    } else {
        TuePosRecAux(&((*L)->suite), pos + 1);
    }
}

void TuePosRec(Liste *L) {
    TuePosRecAux(L, 1);
}

/*******/

// Version itérative avec un pointeur de pointeur : L désigne
// toujours le champ qui pointe sur le bloc courant.

void TuePosIt(Liste *L) {
    int pos = 1;
    while (*L != NULL) {
        if ((*L)->valeur == pos)
            depile(L);
        else
            L = &((*L)->suite);
        pos++;
    }
}


/********************************************/
/*                                          */
/*            TueRetroPos                   */
/*                                          */
/********************************************/

// Une seule passe : la récursion descend jusqu'au bout de la liste,
// et la rétro-position (1 pour le dernier) est calculée en remontant.

static int TueRetroPosAux(Liste *L) {
    if (*L == NULL) return 0;

    int r = 1 + TueRetroPosAux(&((*L)->suite));   // rétro-position de *L
    if ((*L)->valeur == r)
        depile(L);
    return r;
}

void TueRetroPos(Liste *L) {
    TueRetroPosAux(L);
}



/*************************************************/
/*                                               */
/*           Main : exemples de l'énoncé         */
/*                                               */
/*************************************************/

// Construit une liste à partir d'un tableau (pour les tests)
Liste DepuisTableau(int *T, int n)
{
    Liste l = NULL;
    for (int i = n - 1; i >= 0; i--)
        empile(T[i], &l);
    return l;
}

int main() {
    int A[] = {23, 19, 42, 4, 2}, B[] = {2, -2}, C[] = {2, 3, 27, 1}, D[] = {2};
    Liste a = DepuisTableau(A, 5), b = DepuisTableau(B, 2);
    Liste c = DepuisTableau(C, 4), d = DepuisTableau(D, 1);

    printf("----- UnPlusDeuxEgalTrois -----\n");
    printf("[23,19,42,4,2] : %d   [2,-2] : %d   [2,3,27,1] : %d   [2] : %d\n",
           UnPlusDeuxEgalTrois(a), UnPlusDeuxEgalTrois(b),
           UnPlusDeuxEgalTrois(c), UnPlusDeuxEgalTrois(d));

    printf("----- PlusCourte -----\n");
    printf("[2] < [2,-2]  : rec %d  iter %d\n", PlusCourteRec(d, b), PlusCourteIter(d, b));
    printf("[2,-2] < [2]  : rec %d  iter %d\n", PlusCourteRec(b, d), PlusCourteIter(b, d));

    int Z[] = {2, 0, 0, 7, 0, 6, 2, 4, 0};
    Liste z = DepuisTableau(Z, 9);
    printf("----- Verifiek0 sur [2,0,0,7,0,6,2,4,0] -----\n");
    printf("k=4 : rec %d  iter %d     k=1 : rec %d  iter %d\n",
           VerifiekORec(z, 4), VerifiekOIter(z, 4), VerifiekORec(z, 1), VerifiekOIter(z, 1));

    int T[] = {3, 2, 9, 5, 0, 6, 0};
    Liste t = DepuisTableau(T, 7);
    printf("----- NombreTermesAvantZero sur [3,2,9,5,0,6,0] (attendu 4) -----\n");
    printf("It %d  Rec %d  RTSF %d  RTSP %d\n",
           NTAZ_It(t), NTAZ_Rec(t), NTAZ_RTSF(t), NTAZ_RTSP(t));

    int P[] = {0, 4, 3, 9, 5, 0, 9, 2, 1};
    Liste l;

    printf("----- TuePos sur [0,4,3,9,5,0,9,2,1] (attendu 0 4 9 0 9 2 1) -----\n");
    l = DepuisTableau(P, 9); TuePosRec(&l); printf("Rec : "); affiche_iter(l); VideListe(&l);
    l = DepuisTableau(P, 9); TuePosIt(&l);  printf("It  : "); affiche_iter(l); VideListe(&l);

    printf("----- TueRetroPos sur [0,4,3,9,5,0,9,2,1] (attendu 0 4 3 9 0 9) -----\n");
    l = DepuisTableau(P, 9); TueRetroPos(&l); affiche_iter(l); VideListe(&l);

    VideListe(&a); VideListe(&b); VideListe(&c); VideListe(&d);
    VideListe(&z); VideListe(&t);
    return 0;
}
