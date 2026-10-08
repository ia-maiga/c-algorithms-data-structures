/*
 * Part 1 — Simple computations: factorial, e, floating-point precision,
 *          Syracuse sequence and permutations.
 *
 * Authors : Walid Bouzid & Ibrahim Aboubakarine Maiga
 * Context : Algorithms course project, Université Paris-Saclay (2025-2026)
 *
 * Function names follow the original assignment (in French).
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*************************************************/
/*                                               */
/*                type booléen                   */
/*                                               */
/*************************************************/

typedef enum {false, true} bool;

/*************************************************/
/*                                               */
/*            factorielle                        */
/*                                               */
/*************************************************/

long fact1 (int n)
{ if (n==0) return 1 ;
  else return fact1(n-1) * n ;
}

/*************************************************/

// itou avec un argument out => passage par adresse
             // le calcul se fait comme dans la version récursive
             // (...(((1*1)*2)*3) ... *n)
void bisfact2(int n, long * r)
{ if (n==0)
         *r = 1 ;
  else { bisfact2(n-1,r) ;
         *r = n**r ;  // ou *r*n
       }        // Notez le double sens de * ...
}

long fact2 (int n)
{ long r ;
  bisfact2(n,&r) ;
  return r ;
}

/*************************************************/

// variante qui consiste en fait à initialiser r beaucoup plus tôt
// du coup, r est inout

void bisfact3(int n, long * r)
{ if (n==0) {}
  else { bisfact3(n-1,r) ;
         *r = n**r ;  }
}

long fact3 (int n)
{ long r = 1 ;
  bisfact3(n,&r) ;
  return r ;
}

/*************************************************/

// Ici r est vraiment inout, récursif terminal
// le calcul est différent : (((...((1*n)*(n-1)) ... *3)*2)*1)

void bisfact4(int n, long * r)
{ if (n==0) {}
  else { *r = n**r ;
        bisfact4(n-1,r) ; }
}

long fact4 (int n)
{ long r = 1 ;
  bisfact4(n,&r) ;
  return r ;
}

/*************************************************/

// Version volontairement FAUSSE (donnée par l'énoncé) :
// *r est lu avant d'avoir été initialisé, puis écrasé par 1 à la fin.
// Elle rend toujours 1. Conservée pour illustrer l'erreur.

void bisfact5(int n, long * r)
{ if (n==0) *r = 1 ;
  else { *r = n**r ;
        bisfact5(n-1,r) ; }
}

long fact5 (int n)
{ long r = 1 ;   // initialisé pour éviter un comportement indéfini
  bisfact5(n,&r) ;
  return r ;
}

/*************************************************/

// itou en stockant tout dans un tableau (coûteux en espace,
// c'est juste pour manipuler un peu les tableaux)

long fact6(int n)
{ long * T = (long *) malloc((n+1)*sizeof(long)) ;
  T[0] = 1 ;
  for (int i=1 ; i <= n ; i++)
          T[i] = i* T[i-1] ;
  long r = T[n] ;
  free(T) ;
  return r ;
}

/*************************************************/

#define VersionsFact 6

long fact(int n , int v )   // numéro de version
{ switch (v)
   {
   case 1 : return fact1(n) ;
   case 2 : return fact2(n) ;
   case 3 : return fact3(n) ;
   case 4 : return fact4(n) ;
   case 5 : return fact5(n) ;
   case 6 : return fact6(n) ;
   default : return 0 ;
   }
}


/*************************************************/
/*                                               */
/*            Calcul de e                        */
/*                                               */
/*************************************************/

// e = somme des 1/n!  On ne recalcule pas n! à chaque tour :
// le terme 1/n! s'obtient en divisant le terme précédent par n.
// On s'arrête quand ajouter le terme ne change plus la somme
// (limite de précision du type).

float Efloat() {
    float sum = 1.0f;
    float term = 1.0f;
    for (int n = 1; n < 1000; n++) {
        term /= n;
        if (sum + term == sum) break;
        sum += term;
    }
    return sum;
}

/*************************************************/

double Edouble() {
    double sum = 1.0;
    double term = 1.0;
    for (int n = 1; n < 1000; n++) {
        term /= n;
        if (sum + term == sum) break;
        sum += term;
    }
    return sum;
}

/*************************************************/

long double Elongdouble() {
    long double sum = 1.0L;
    long double term = 1.0L;
    for (int n = 1; n < 1000; n++) {
        term /= n;
        if (sum + term == sum) break;
        sum += term;
    }
    return sum;
}

/*************************************************/
/*                                               */
/*            Suite Y                            */
/*                                               */
/*************************************************/

// y0 = e - 1, yn = n*y(n-1) - 1. Mathématiquement yn -> 0,
// mais l'erreur initiale eps est multipliée par n! : la suite
// finit par diverger, d'autant plus tard que le type est précis.

void afficheYfloat (int n){
  float y = Efloat() - 1.0f;
  printf("y[0] = %f\n", y);
  for (int i = 1; i < n; i++){
    y = i * y - 1;
    printf("y[%d] = %f\n", i, y);
  }
}

/*************************************************/

void afficheYdouble (int n){
  double y = Edouble() - 1.0;
  printf("y[0] = %f\n", y);
  for (int i = 1; i < n; i++){
    y = i * y - 1;
    printf("y[%d] = %f\n", i, y);
  }
}

/*************************************************/

void afficheYlongdouble (int n){
  long double y = Elongdouble() - 1.0L;
  printf("y[0] = %Lf\n", y);
  for (int i = 1; i < n; i++){
    y = i * y - 1.0L;
    printf("y[%d] = %Lf\n", i, y);
  }
}

/*************************************************/
/*                                               */
/*          Permutations tableaux                */
/*                                               */
/*************************************************/

int Int_Lire ()     // Lecture simpliste d'un entier positif ou nul
                    // ignore les char avant le premier chiffre
                    // s'arrête en absorbant le char après le dernier chiffre
                    // exemple "truc bidule 345x" donne 345
{ int c ;
  do c = getchar() ; while ( c != EOF && (c<'0' || c>'9')) ;
  if (c == EOF) return 0 ;
  int n = c - '0' ;
  while (true)
  { c = getchar() ;
    if ( c<'0' || c>'9') break ;
    n = n*10 + (c - '0') ;
  }
  return n ;
}

/*************************************************/

int* P_Lire (int n)
{ printf("Input Permutation 0..%d : ", n-1) ;
  int* T = (int*) malloc(n*sizeof(int)) ;
  for (int i=0 ; i<n ; i++) T[i] = Int_Lire() ;
  return T ;
}

/*************************************************/

void P_Affiche (int* P , int n)
{ printf("[") ;
  for (int i=0 ; i<n ; i++) printf(" %d",P[i]) ;
  printf(" ]\n") ;
}

/*************************************************/

int* P_identite (int n)
{ int* T = (int*) malloc(n*sizeof(int)) ;
  for (int i=0 ; i<n ; i++) T[i] = i ;
  return T ;
}

/*************************************************/

int* P_Inverse (int* P , int n)
{ int* T = (int*) malloc(n*sizeof(int));
  for (int i = 0; i<n; i++)
    T[P[i]] = i;
  return T;
}

/*************************************************/

void P_Compose(int* P, int* Q, int* R , int n)  // écrit PoQ dans R
{ for (int i = 0; i<n; i++)
    R[i] = P[Q[i]];
}

/*************************************************/

// Vrai ssi P est une bijection de [0..n[ : chaque valeur est
// dans [0..n[ et n'apparaît qu'une seule fois.

bool P_Verifie (int* P , int n)
{ bool* dejavu = (bool*) calloc(n, sizeof(bool)) ;
  bool ok = true ;
  for (int i = 0; i<n && ok; i++)
  { if (P[i] < 0 || P[i] >= n || dejavu[P[i]])
      ok = false ;
    else
      dejavu[P[i]] = true ;
  }
  free(dejavu) ;
  return ok ;
}

/*************************************************/

// Version 1 : récursive, P^k = P^(k-1) o P, complexité en k

int* P_power1(int* P, int n, int k)
{ if (k == 0) return P_identite(n) ;
  int* prec = P_power1(P, n, k-1) ;
  int* R = (int*) malloc(n*sizeof(int)) ;
  P_Compose(prec, P, R, n) ;
  free(prec) ;
  return R ;
}

/**********************/

// Version 2 : itérative, P^k = P^(k-1) o P, complexité en k
// Deux tableaux seulement, échangés à chaque tour.

int* P_power2(int* P, int n, int k)
{ int* R   = P_identite(n) ;
  int* tmp = (int*) malloc(n*sizeof(int)) ;
  for (int i = 0; i < k; i++)
  { P_Compose(R, P, tmp, n) ;
    int* swap = R ; R = tmp ; tmp = swap ;
  }
  free(tmp) ;
  return R ;
}

/**********************/

// Version 3 : récursive, exponentiation rapide, complexité en log2(k)
// P^k = (P^(k/2))^2 si k pair, (P^(k/2))^2 o P sinon

int* P_power3(int* P, int n, int k)
{ if (k == 0) return P_identite(n) ;
  int* half = P_power3(P, n, k/2) ;
  int* R = (int*) malloc(n*sizeof(int)) ;
  P_Compose(half, half, R, n) ;
  if (k % 2 == 1)
  { P_Compose(R, P, half, n) ;      // on réutilise half comme tampon
    int* swap = R ; R = half ; half = swap ;
  }
  free(half) ;
  return R ;
}

/**********************/

// Version 4 : itérative, exponentiation rapide, complexité en log2(k)
// On parcourt les bits de k ; base = P^(2^i).

int* P_power4(int* P, int n, int k)
{ int* R    = P_identite(n) ;
  int* base = (int*) malloc(n*sizeof(int)) ;
  int* tmp  = (int*) malloc(n*sizeof(int)) ;
  for (int i = 0; i < n; i++) base[i] = P[i] ;

  while (k > 0)
  { if (k % 2 == 1)
    { P_Compose(R, base, tmp, n) ;
      int* swap = R ; R = tmp ; tmp = swap ;
    }
    P_Compose(base, base, tmp, n) ;
    int* swap = base ; base = tmp ; tmp = swap ;
    k = k / 2 ;
  }
  free(base) ; free(tmp) ;
  return R ;
}

/*************************************************/

#define VersionsPuissance 4

int* P_power(int* P, int n, int k, int v )   // version v de 1 à VersionsPuissance
{ switch (v)
   {
   case 1 : return P_power1(P,n,k) ;
   case 2 : return P_power2(P,n,k) ;
   case 3 : return P_power3(P,n,k) ;
   case 4 : return P_power4(P,n,k) ;
   default : return NULL ;
   }
}

/*************************************************/

// Permutation aléatoire uniforme : mélange de Fisher-Yates.
// Chacune des n! permutations sort avec la même probabilité.
// (srand doit être appelé une seule fois, dans le main.)

int* P_random (int n)
{ int* T = P_identite(n) ;
  for (int i = n-1; i > 0; i--)
  { int j = rand() % (i+1) ;
    int tmp = T[i] ; T[i] = T[j] ; T[j] = tmp ;
  }
  return T ;
}


/*************************************************/
/*                                               */
/*            Syracuse                           */
/*                                               */
/*************************************************/

#define CSyr 2025

/*************************************************/

// Version itérative

int SyracuseI (int n){
  int x = CSyr;
  for (int i = 0; i < n; ++i) {
      if (x % 2 == 0) x = x / 2;
      else x = 3 * x + 1;
  }
  return x;
}

/*************************************************/

// Version récursive terminale avec sous-fonction

static int aux_SF(int cur, int reste){
    if (reste == 0) return cur;
    if (cur % 2 == 0) cur = cur / 2;
    else cur = 3 * cur + 1;
    return aux_SF(cur, reste - 1);
}

int SyracuseSF(int n){
    return aux_SF(CSyr, n);
}

/*************************************************/

// Version récursive terminale avec sous-procédure (argument inout)

static void SyracuseSP_rec(int *val, int n){
    if (n == 0) return;
    if (*val % 2 == 0)
        *val = *val / 2;
    else
        *val = 3 * (*val) + 1;
    SyracuseSP_rec(val, n - 1);
}

int SyracuseSP(int n){
    int val = CSyr;
    SyracuseSP_rec(&val, n);
    return val;
}

/*************************************************/

// Version récursive sans sous-fonctionnalité

int SyracuseR(int n){
    if (n == 0) return CSyr;
    int prev = SyracuseR(n - 1);
    if (prev % 2 == 0) return prev / 2;
    else return 3 * prev + 1;
}

/*************************************************/

#define VersionsSyracuse 4

int Syracuse (int n, int i)
{ switch (i)
   {
   case 1 : return SyracuseI  (n) ;
   case 2 : return SyracuseSF (n) ;
   case 3 : return SyracuseSP (n) ;
   case 4 : return SyracuseR  (n) ;
   default : return 0 ;
   }
}


/*************************************************/
/*                                               */
/*               main                            */
/*                                               */
/*************************************************/

int main()
{
  srand((unsigned) time(NULL)) ;

/************************  taille des nombres  *************************/

  printf("=== Tailles des types (dépendent du compilateur) ===\n") ;
  printf("short : %d octets\n", (int) sizeof(short));
  printf("int : %d octets\n", (int) sizeof(int));
  printf("long : %d octets\n", (int) sizeof(long));
  printf("long long : %d octets\n", (int) sizeof(long long));
  printf("float : %d octets\n", (int) sizeof(float));
  printf("double : %d octets\n", (int) sizeof(double));
  printf("long double : %d octets\n\n", (int) sizeof(long double));

/************************  factorielle  *************************/

  printf("=== Factorielles de 0, 1, 2, 3, 4, 5, 10, 15, 20 ===\n") ;
  printf("(20! est la plus grande qui tient dans un long 64 bits ; la version 5 est fausse exprès)\n") ;
  for (int v=1 ; v<=VersionsFact ; v++ )
     printf("version %d : %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",
         v, fact(0,v), fact(1,v), fact(2,v), fact(3,v), fact(4,v),
         fact(5,v), fact(10,v), fact(15,v), fact(20,v)) ;
  printf("\n") ;

/******************    Autour de e      *******************************/

  printf("=== Calcul de e ===\n") ;
  printf("float       : %.20f\n", Efloat()) ;
  printf("double      : %.30f\n", Edouble()) ;
  printf("long double : %.40Lf\n\n", Elongdouble()) ;

  printf("=== Suite y(n) = n*y(n-1) - 1, en float / double / long double ===\n") ;
  afficheYfloat(30) ;
  afficheYdouble(30) ;
  afficheYlongdouble(30) ;
  printf("\n") ;

/******************************* Permutations **************************/

  printf("=== Permutations ===\n") ;
  int n = 6 ;
  int P[6] = {0, 5, 3, 4, 2, 1} ;
  int Q[6] = {4, 5, 2, 1, 3, 0} ;
  int R[6] ;

  printf("P         = ") ; P_Affiche(P, n) ;
  printf("Q         = ") ; P_Affiche(Q, n) ;
  int* Qinv = P_Inverse(Q, n) ;
  printf("Q^-1      = ") ; P_Affiche(Qinv, n) ;   // attendu [5,3,2,4,0,1]
  P_Compose(P, Q, R, n) ;
  printf("P o Q     = ") ; P_Affiche(R, n) ;      // attendu [2,1,3,5,4,0]
  free(Qinv) ;

  int bad1[4] = {1, 0, 3, 1} ;
  int bad2[4] = {0, 1, 23, 42} ;
  printf("P_Verifie(P) = %d, P_Verifie([1,0,3,1]) = %d, P_Verifie([0,1,23,42]) = %d\n",
         P_Verifie(P, n), P_Verifie(bad1, 4), P_Verifie(bad2, 4)) ;

  int* Rnd = P_random(n) ;
  printf("aléatoire = ") ; P_Affiche(Rnd, n) ;

  printf("Puissances 0, 1, 2, 3, n, n! de la permutation aléatoire :\n") ;
  int exposants[6] = {0, 1, 2, 3, n, (int) fact1(n)} ;
  for (int v = 1 ; v <= VersionsPuissance ; v++)
  { printf("version %d\n", v) ;
    for (int e = 0 ; e < 6 ; e++)
    { int* Pk = P_power(Rnd, n, exposants[e], v) ;
      printf("  ^%-4d ", exposants[e]) ; P_Affiche(Pk, n) ;
      free(Pk) ;
    }
  }
  free(Rnd) ;
  printf("\n") ;

/******************    Syracuse    *******************************/

  printf("=== Syracuse (CSyr = %d) ===\n", CSyr) ;
  printf("attendu : Syr(0)=2025 Syr(3)=1519 Syr(10)=15388 Syr(100)=638 Syr(1000)=4\n") ;
  int tests[5] = {0, 3, 10, 100, 1000} ;
  for (int v = 1 ; v <= VersionsSyracuse ; v++)
  { printf("version %d :", v) ;
    for (int j = 0 ; j < 5 ; j++)
      printf(" Syr(%d)=%d", tests[j], Syracuse(tests[j], v)) ;
    printf("\n") ;
  }

  return 0;
}
