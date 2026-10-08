/*
 * Part 3 — Quadtrees: black & white images as 4-ary trees
 *          (parsing, display, simplification, inclusion, grey
 *          sub-images and maze traversal).
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
/*                type booléen                  */
/*                                               */
/*************************************************/

typedef enum {false, true} bool;


/*************************************************/
/*                                               */
/*          definition type arbre                */
/*                                               */
/*************************************************/

typedef struct bloc_image
    { 
        bool blanc ;
        struct bloc_image * Im[4] ;
    } bloc_image ;

typedef bloc_image *image ;

/*************************************************/
/*                                               */
/*                 Les bases                     */
/*                                               */
/*************************************************/

image Wht() {
    image res = malloc(sizeof(bloc_image));
    res->blanc = true;
    for(int i = 0; i<4; i++) {
        res->Im[i] = NULL;
    }
    return res; 
} 

image Blk() {
    return NULL;
}

image Cut(bloc_image* i0, bloc_image* i1, bloc_image* i2, bloc_image* i3) {
    image res = malloc(sizeof(bloc_image));
    res->blanc = false;
    res->Im[0] = i0; res->Im[1] = i1; res->Im[2] = i2; res->Im[3] = i3;
    return res;
}

/*************************************************/
/*                                               */
/*             Libération mémoire                */
/*                                               */
/*************************************************/

void FreeImage(image img) {
    if (img == NULL) return;          // image noire : rien à libérer
    if (!img->blanc)
        for (int i = 0; i < 4; i++)
            FreeImage(img->Im[i]);
    free(img);
}

/*************************************************/
/*                                               */
/*                    Affiche                    */
/*                                               */
/*************************************************/

void Affiche(image i) {
    if(i == NULL) {
        printf("Z");
    }
    else if(i->blanc) {
        printf("o");
    }
    else {
        printf("*");
        for (int k = 0; k < 4; k++)
            Affiche(i->Im[k]);
    }
}

/*************************************************/
/*                                               */
/*                ProfAffiche                    */
/*                                               */
/*************************************************/

void ProfAfficheRec(image i, int d) {
    if (i == NULL) {
        printf("Z%d ", d);
    }
    else if (i->blanc) {
        printf("o%d ", d);
    }
    else {
        printf("*%d ", d);
        for (int k = 0; k < 4; k++)
            ProfAfficheRec(i->Im[k], d+1);
    }
}

void ProfAffiche(image i) {
    ProfAfficheRec(i, 0);
}

/*************************************************/
/*                                               */
/*                    Lecture                    */
/*                                               */
/*************************************************/

image LectureRec() {
    int c;
    do {
        c = getchar();
    } while (c != 'Z' && c != 'o' && c != '*');
    if (c == 'Z')
        return Blk();
    if (c == 'o')
        return Wht();
    
    image i0 = LectureRec();
    image i1 = LectureRec();
    image i2 = LectureRec();
    image i3 = LectureRec();

    return Cut(i0, i1, i2, i3);
}

image Lecture() {
    return LectureRec();
}

/*******/

// Même lecture, mais depuis une chaîne de caractères (pratique pour
// les tests). *s avance au fur et à mesure de la lecture.

static image LectureChaineRec(const char **s) {
    char c;
    do {
        c = *(*s)++;
        if (c == '\0') return Blk();     // chaîne malformée
    } while (c != 'Z' && c != 'o' && c != '*');

    if (c == 'Z') return Blk();
    if (c == 'o') return Wht();

    image i0 = LectureChaineRec(s);
    image i1 = LectureChaineRec(s);
    image i2 = LectureChaineRec(s);
    image i3 = LectureChaineRec(s);
    return Cut(i0, i1, i2, i3);
}

image LectureChaine(const char *s) {
    return LectureChaineRec(&s);
}


/*************************************************/
/*                                               */
/*           Dessin noir et blanc?               */
/*                                               */
/*************************************************/

bool DessinNoir(image I) {
    if (I == NULL) 
        return true; 
    if (I->blanc == true)
        return false;
    return DessinNoir(I->Im[0]) &&
           DessinNoir(I->Im[1]) &&
           DessinNoir(I->Im[2]) &&
           DessinNoir(I->Im[3]);
}

bool DessinBlanc(image I) {
    if (I == NULL)              // une image noire n'est pas blanche
        return false;
    if (I->blanc == true)
        return true;
    return DessinBlanc(I->Im[0]) &&
           DessinBlanc(I->Im[1]) &&
           DessinBlanc(I->Im[2]) &&
           DessinBlanc(I->Im[3]);
}

/*************************************************/
/*                                               */
/*                 QuotaNoir                     */
/*                                               */
/*************************************************/

double QuotaNoir(image I) {
    if (I == NULL)
        return 1.0;  
    if (I->blanc)
        return 0.0;
    double q0 = QuotaNoir(I->Im[0]);
    double q1 = QuotaNoir(I->Im[1]);
    double q2 = QuotaNoir(I->Im[2]);
    double q3 = QuotaNoir(I->Im[3]);
    return (q0 + q1 + q2 + q3) / 4.0;
}

/*************************************************/
/*                                               */
/*                 Copie                         */
/*                                               */
/*************************************************/

image Copie(image I) {
    if (I == NULL)
        return Blk();
    if (I->blanc)
        return Wht();
    image c0 = Copie(I->Im[0]);
    image c1 = Copie(I->Im[1]);
    image c2 = Copie(I->Im[2]);
    image c3 = Copie(I->Im[3]);
    return Cut(c0, c1, c2, c3);
}

/*************************************************/
/*                                               */
/*                 Diagonale                     */
/*                                               */
/*************************************************/

image Diagonale(int p)
{
    if (p == 0)
        return Blk();

    image d = Diagonale(p - 1);

    // Copie(d) plutôt que d : chaque bloc n'appartient qu'à un seul arbre,
    // ce qui permet de libérer l'image sans double free.
    return Cut(d, Wht(), Wht(), Copie(d));
}


/*************************************************/
/*                                               */
/*                 Simplifie Prof                */
/*                                               */
/*************************************************/

void SimplifieProfP(image *I, int p)
{
    if (*I == NULL || (*I)->blanc)
        return;
    if (p == 0) {
        if (DessinNoir(*I)) {
            FreeImage(*I);
            *I = Blk();
        } else if (DessinBlanc(*I)) {
            FreeImage(*I);
            *I = Wht();
        }
        return;
    }
    for (int k = 0; k < 4; k++)
        SimplifieProfP(&((*I)->Im[k]), p - 1);
}

/*************************************************/
/*                                               */
/*                 Incluse                       */
/*                                               */
/*************************************************/

bool Incluse(image I1, image I2)
{
    if (I1 == NULL)
        return (I2 == NULL);
    if (I1->blanc)
        return true;
    if (I2 == NULL)
        return true;
    if (I2->blanc)
        return false;
    for (int k = 0; k < 4; k++)
        if (!Incluse(I1->Im[k], I2->Im[k]))
            return false;

    return true;
}

/*************************************************/
/*                                               */
/*         Compte Sous Images Grises             */
/*                                               */
/*************************************************/
// Une image est grise si son quota de noir est dans [1/3, 2/3].
// Un seul parcours : la sous-fonction rend le quota de noir de
// l'image et incrémente le compteur au passage (complexité linéaire).

static double CompteGrisesAux(image I, int *cpt) {
    if (I == NULL) return 1.0;      // noir
    if (I->blanc)  return 0.0;      // blanc

    double q = 0.0;
    for (int k = 0; k < 4; k++)
        q += CompteGrisesAux(I->Im[k], cpt);
    q = q / 4.0;

    if (q >= 1.0/3.0 && q <= 2.0/3.0)
        (*cpt)++;
    return q;
}

int CompteSousImagesGrises(image I) {
    int cpt = 0;
    CompteGrisesAux(I, &cpt);
    return cpt;
}



/*************************************************/
/*                                               */
/*                 Labyrinthe                    */
/*                                               */
/*************************************************/
int Profondeur(image img) {
    if (img == NULL || img->blanc) return 0;
    int p = 0;
    for (int i = 0; i < 4; i++) {
        int pi = Profondeur(img->Im[i]);
        if (pi > p) p = pi;
    }
    return 1 + p;
}


void RemplitGrille(image img, int x, int y, int taille, bool **grille) {
    if (img == NULL) {
        // noir → murs
        return;
    }

    if (img->blanc) {
        for (int i = x; i < x + taille; i++)
            for (int j = y; j < y + taille; j++)
                grille[i][j] = true;
        return;
    }

    int t2 = taille / 2;
    RemplitGrille(img->Im[0], x,      y,      t2, grille);
    RemplitGrille(img->Im[1], x,      y+t2,   t2, grille);
    RemplitGrille(img->Im[2], x+t2,   y,      t2, grille);
    RemplitGrille(img->Im[3], x+t2,   y+t2,   t2, grille);
}

bool LabyrintheDFS(int x, int y, bool **visited, bool **grille, int n) {
    if (x < 0 || x >= n || y < 0 || y >= n) return false;
    if (visited[x][y] || !grille[x][y]) return false;
    if (x == n-1 && y == n-1) return true;
    visited[x][y] = true;
    return LabyrintheDFS(x+1, y, visited, grille, n) ||
           LabyrintheDFS(x-1, y, visited, grille, n) ||
           LabyrintheDFS(x, y+1, visited, grille, n) ||
           LabyrintheDFS(x, y-1, visited, grille, n);
}

// On transforme l'arbre en grille n x n de pixels (n = 2^profondeur),
// puis on cherche un chemin blanc du coin haut-gauche au coin
// bas-droite par un parcours en profondeur (DFS).

bool Labyrinthe(image img) {
    if (img == NULL) return false;                  // tout noir

    int n = 1 << Profondeur(img);
    bool **grille  = malloc(n * sizeof(bool *));
    bool **visited = malloc(n * sizeof(bool *));
    for (int i = 0; i < n; i++) {
        grille[i]  = calloc(n, sizeof(bool));       // false = mur
        visited[i] = calloc(n, sizeof(bool));
    }

    RemplitGrille(img, 0, 0, n, grille);
    bool res = LabyrintheDFS(0, 0, visited, grille, n);

    for (int i = 0; i < n; i++) {
        free(grille[i]);
        free(visited[i]);
    }
    free(grille);
    free(visited);
    return res;
}

/*************************************************/
/*                                               */
/*      Bonus : dessin de l'image en ASCII       */
/*                                               */
/*************************************************/

// Affiche l'image pixel par pixel : '#' pour noir, '.' pour blanc.

void AfficheGrille(image img) {
    int n = 1 << Profondeur(img);
    bool **grille = malloc(n * sizeof(bool *));
    for (int i = 0; i < n; i++)
        grille[i] = calloc(n, sizeof(bool));

    RemplitGrille(img, 0, 0, n, grille);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%c ", grille[i][j] ? '.' : '#');
        printf("\n");
        free(grille[i]);
    }
    free(grille);
}

/*************************************************/
/*                                               */
/*           Main : exemples de l'énoncé         */
/*                                               */
/*************************************************/

int main(int argc, char **argv) {

    // Mode interactif : ./part3 -i  puis taper une image, ex. *oZ*ooZoZ
    if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'i') {
        printf("Image (notation o / Z / *) : ");
        image I = Lecture();
        printf("Affiche      : "); Affiche(I);     printf("\n");
        printf("ProfAffiche  : "); ProfAffiche(I); printf("\n");
        printf("QuotaNoir    : %.4f\n", QuotaNoir(I));
        printf("Grises       : %d\n", CompteSousImagesGrises(I));
        printf("Labyrinthe   : %s\n", Labyrinthe(I) ? "traversable" : "bloqué");
        AfficheGrille(I);
        FreeImage(I);
        return 0;
    }

    image I;

    printf("----- ProfAffiche -----\n");
    I = LectureChaine("*Z*ooZoo*Z*ZZo*ZoZZoZ");
    ProfAffiche(I); printf("\n");
    FreeImage(I);

    printf("----- DessinBlanc / DessinNoir -----\n");
    I = LectureChaine("**o*oooo*ooooooo*oooo");
    printf("**o*oooo*ooooooo*oooo : blanc %d, noir %d\n", DessinBlanc(I), DessinNoir(I));
    FreeImage(I);

    printf("----- QuotaNoir (attendu 0.75) -----\n");
    I = LectureChaine("*Z*oZooZ*ZZZo");
    printf("%.2f\n", QuotaNoir(I));
    FreeImage(I);

    printf("----- Diagonale(3) -----\n");
    I = Diagonale(3);
    Affiche(I); printf("\n");
    image C = Copie(I);
    FreeImage(I); FreeImage(C);

    printf("----- SimplifieProfP(p = 2) -----\n");
    I = LectureChaine("*(*ZZZZ)(*Zo(*Z(*Z(*ZZZZ)(*ZZZZ)(*ZZZZ))ZZ)o)"
                      "(*ZoZ(*ZoZ(*oooo)))(*oo(*oooo)o)");
    SimplifieProfP(&I, 2);
    Affiche(I); printf("\n");
    FreeImage(I);

    printf("----- Incluse (attendu 0) -----\n");
    image A = LectureChaine("***ooooZoZoZ**ooZZoo*ZooZ");
    image B = LectureChaine("**oZZZ*ooZo*ZZZZ*ZoZ*ZZZo");
    printf("%d\n", Incluse(A, B));
    FreeImage(A); FreeImage(B);

    printf("----- CompteSousImagesGrises (attendu 4) -----\n");
    I = LectureChaine("*oZ*Z*oZooo*Zooo *Z*oZooo*ZZoo");
    printf("%d\n", CompteSousImagesGrises(I));
    FreeImage(I);

    printf("----- Labyrinthe (attendu 1 puis 0) -----\n");
    A = LectureChaine("***ooZo**ZZoooZZ*Zoo*ZooZZ ***ooZZoZZ*o*ooZoo*Zooo*oZ*oooZo"
                      "*Z*oZoZoo *Z**ooZoooZZ*oooZ **oZZZZ*oooZ*oZoo");
    B = LectureChaine("***ooZo**ZZoooZZ*Zoo*ZooZZ ***ooZZoZZ*o*ooZoo*Zooo*oZ*ZooZo"
                      "*Z*oZoZoo *Z**ooZoooZZ*oooZ **oZZZZ*oooZ*oZoo");
    printf("%d %d\n", Labyrinthe(A), Labyrinthe(B));
    printf("Premier labyrinthe :\n");
    AfficheGrille(A);
    FreeImage(A); FreeImage(B);

    return 0;
}
