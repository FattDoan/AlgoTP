/**************************************************************************
 *                     RENDU DU TP ALGO : Partie 2Ter                     *   
 *  Noms du binôme:                                                       *
 *  - Thanh Phat DOAN (thanh-phat.doan@universite-paris-saclay.fr)        *
 *  - Naël EL YAZGHI  (nael.el-yazghi@universite-paris-saclay.fr)         *
 *                                                                        *               
 *  Le code est conforme à la norme C99 et a été compilé avec les options *               
 *  les plus strictes: -std=c99 -pedantic -Wall -Wextra -Werror.          *
 *  Les fuites mémoire ont été vérifiées avec Valgrind (--leak-check=full)*
 *                                                                        *
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/***************************************
 *                                     *
 *       Briques de base               *
 *                                     *
 ***************************************/

typedef struct Bloc
{
    int valeur;
    struct Bloc * suite;
} Bloc;

typedef enum {false, true} bool;    
// NOTE: gcc peut se plaindre si on inclut pas -std=c99

Bloc* ajoute(int x, Bloc* suite) {
    Bloc* tmp = malloc(sizeof(Bloc));
    tmp->valeur = x;
    tmp->suite = suite;
    return tmp;
}

/*******************************************************************
 *                                                                 *
 *              Variante de pointeur sortie                        *
 *                       et pointeur de pointeur entree            *
 *                                                                 *
 *******************************************************************/

typedef struct File1 {
    Bloc* sortie;
    Bloc** entree;
} File1;

/*******************************************************************/
// Si la file est vide, entree doit avoir l'adresse de sortie
File1* file1Vide() {
    File1* f = malloc(sizeof(File1));
    f->sortie = NULL;
    f->entree = &(f->sortie);   
    return f;
}
bool estVideFile1(File1* f) {
    return f->sortie == NULL && f->entree == &(f->sortie);
}

/*******************************************************************
 *                                                                 *
 *                             entree1                             *            
 *                                                                 *
 *******************************************************************/
// entree a toujours l'adresse du bloc suite du dernier bloc,
// dans ce cas, new
void entree1(int x, File1* f) {
    Bloc* new = ajoute(x, NULL);
    *(f->entree) = new;
    f->entree = &(new->suite);
}

/*******************************************************************
 *                                                                 *
 *                             sortie1                             *            
 *                                                                 *
 *******************************************************************/
// L'idée est celle de depile(), cepedant il faut faire attention
// au cas de limite où f->sortie == NULL, c-a-d la file est vide
void sortie1(int* x, File1* f) {
    assert(f != NULL);
    
    Bloc* tmp = f->sortie;
    *x = tmp->valeur;
    f->sortie = f->sortie->suite;
    free(tmp);

    if (f->sortie == NULL) {
        f->entree = &(f->sortie);
    }
}

/*******************************************************************/
// Parcour iterative depuis f->sortie jusqu'a` la fin (NULL)
void afficheFile1(File1* f) {
    assert(f != NULL);

    printf("[");
    Bloc* cur = f->sortie;
    while (cur != NULL) {
        printf(" %d", cur->valeur);
        cur = cur->suite;
        if (cur != NULL) printf(",");
    }
    printf("]\n");
}

/*******************************************************************/
// Le longueur/taille de la file 
int sizeFile1(File1* f) {
    assert(f != NULL);

    int sz = 0;
    Bloc* cur = f->sortie;
    while (cur != NULL) {
        sz++;
        cur = cur->suite; 
    }
    return sz;
}

/*******************************************************************/
// On profite la fonction sortie en sortant tous les éléments 
// au dummy variable x
void videFile1(File1* f) {
    int x;
    while (f->sortie != NULL) sortie1(&x, f);
    free(f);
}

/*******************************************************************
 *                                                                 *
 *              Variante de la liste circulaire                    *            
 *                                                                 *
 *******************************************************************/

typedef struct File2 {
    Bloc* dernier;
} File2;

/*******************************************************************/
// La file est vide <=> f->dernier == NULL
File2* file2Vide() {
    File2* f = malloc(sizeof(File2));
    f->dernier = NULL;
    return f;
}
bool estVideFile2(File2* f) {
    return f->dernier == NULL;
}

/*******************************************************************
 *                                                                 *
 *                             entree2                             *            
 *                                                                 *
 *******************************************************************/
// L'idée: crée un nouveau bloc 
//         pointe vers f->dernier->suite (la tête de la file)
//         le bloc de f->dernier pointe vers ce bloc 
//         f->dernier = ce bloc
// <!> Cas limite: la file est vide -> le bloc pointe vers lui-même
void entree2(int x, File2* f) {
    assert(f != NULL);

    if (f->dernier == NULL) {
        f->dernier = ajoute(x, NULL);
        f->dernier->suite = f->dernier;
        return;
    }
    Bloc* new = ajoute(x, f->dernier->suite);
    f->dernier->suite = new;
    f->dernier = new;
}

/*******************************************************************
 *                                                                 *
 *                             sortie2                             *            
 *                                                                 *
 *******************************************************************/
// Supprimer la tête de la file
// <!> Cas limite : la file ne contient qu'un seul élément 

void sortie2(int* x, File2* f) {
    assert(f != NULL);

    Bloc* tmp = f->dernier->suite;
    *x = tmp->valeur;

    if (tmp == f->dernier) {
        free(tmp);
        f->dernier = NULL;
        return;
    }

    f->dernier->suite = tmp->suite;
    free(tmp);
}

/*******************************************************************/
// On commence a` f->dernier->suite (la tête de la file) et 
// on s'arrête lorsqu'on arrive a` l'original f->dernier (la fin)
void afficheFile2(File2* f) {
    if (f == NULL || f->dernier == NULL) return;
    
    printf("[");
    Bloc* cur = f->dernier->suite;
    Bloc* start = cur;
    do {
        printf(" %d", cur->valeur);
        cur = cur->suite;
        if (cur != start) printf(",");
    } while (cur != start);
    printf("]\n");    
}

/*******************************************************************/
// Le longueur/taille de la file
int sizeFile2(File2* f) {
    assert(f != NULL);

    Bloc* cur = f->dernier;
    Bloc* ori = cur;
    int sz = 0;
    do {
        sz++;
        cur = cur->suite;
    } while (cur != ori);
    return sz;

}

/*******************************************************************/
// même idée que videFile1, on profite la fonciton sortie2()
void videFile2(File2* f) {
    int x;
    while (f->dernier != NULL) sortie2(&x, f);
    free(f);
}

/********************************************************************
 *                              TESTS                               *
 ********************************************************************/
// Tous les tests de File1 et File2 sont identiques pour vérifier
// le même comportement de la file

/******************** FILE1 TESTS *********************/
void TEST1_File1() {
    File1* f = file1Vide();
    entree1(1, f);
    entree1(2, f);
    entree1(3, f);
    entree1(4, f);
    assert(sizeFile1(f) == 4);
    
    int x;
    sortie1(&x, f); assert(x == 1);
    sortie1(&x, f); assert(x == 2);
    sortie1(&x, f); assert(x == 3);
    assert(sizeFile1(f) == 1);
    sortie1(&x, f); assert(x == 4);
    assert(estVideFile1(f));
    videFile1(f);
}

void TEST2_File1() {
    File1* f = file1Vide();
    int x;
    
    entree1(10, f);
    entree1(20, f);
    sortie1(&x, f); assert(x == 10);
    entree1(30, f);
    sortie1(&x, f); assert(x == 20);
    entree1(40, f);
    assert(sizeFile1(f) == 2);
    sortie1(&x, f); assert(x == 30);
    sortie1(&x, f); assert(x == 40);
    assert(estVideFile1(f));
    videFile1(f);
}

// Test de size 
void TEST3_File1() {
    File1* f = file1Vide();
    assert(sizeFile1(f) == 0);
    
    for (int i = 1; i <= 10; i++) {
        entree1(i*10, f);
        assert(sizeFile1(f) == i);
    }
    
    int x;
    for (int i = 10; i >= 1; i--) {
        assert(sizeFile1(f) == i);
        sortie1(&x, f);
    }
    assert(sizeFile1(f) == 0);
    videFile1(f);
}

void TEST4_File1() {
    File1* f = file1Vide();
    int x;
    
    entree1(1, f);
    entree1(2, f);
    sortie1(&x, f);
    sortie1(&x, f);
    assert(estVideFile1(f));
    
    // remplir après avoir vidé
    entree1(100, f);
    entree1(200, f);
    assert(sizeFile1(f) == 2);
    sortie1(&x, f); assert(x == 100);
    sortie1(&x, f); assert(x == 200);
    assert(estVideFile1(f));
    videFile1(f);
}


/******************** FILE2 TESTS *********************/
void TEST1_File2() {
    File2* f = file2Vide();
    entree2(1, f);
    entree2(2, f);
    entree2(3, f);
    entree2(4, f);
    assert(sizeFile2(f) == 4);
    
    int x;
    sortie2(&x, f); assert(x == 1);
    sortie2(&x, f); assert(x == 2);
    sortie2(&x, f); assert(x == 3);
    assert(sizeFile2(f) == 1);
    sortie2(&x, f); assert(x == 4);
    assert(estVideFile2(f));
    videFile2(f);
}

void TEST2_File2() {
    File2* f = file2Vide();
    int x;
    
    entree2(10, f);
    entree2(20, f);
    sortie2(&x, f); assert(x == 10);
    entree2(30, f);
    sortie2(&x, f); assert(x == 20);
    entree2(40, f);
    assert(sizeFile2(f) == 2);
    sortie2(&x, f); assert(x == 30);
    sortie2(&x, f); assert(x == 40);
    assert(estVideFile2(f));
    videFile2(f);
}

// Test de size
void TEST3_File2() {
    File2* f = file2Vide();
    
    for (int i = 1; i <= 10; i++) {
        entree2(i*10, f);
        assert(sizeFile2(f) == i);
    }
    
    int x;
    for (int i = 10; i >= 1; i--) {
        assert(sizeFile2(f) == i);
        sortie2(&x, f);
    }
    videFile2(f);
}

void TEST4_File2() {
    File2* f = file2Vide();
    int x;
    
    entree2(1, f);
    entree2(2, f);
    sortie2(&x, f);
    sortie2(&x, f);
    assert(estVideFile2(f));
    
    // remplir après avoir vidé
    entree2(100, f);
    entree2(200, f);
    assert(sizeFile2(f) == 2);
    sortie2(&x, f); assert(x == 100);
    sortie2(&x, f); assert(x == 200);
    assert(estVideFile2(f));
    videFile2(f);
}

/*************************************************************/
void TESTS_File1() {
    TEST1_File1();
    TEST2_File1();
    TEST3_File1();
    TEST4_File1();
    printf("File1 tests passed!\n");
}

void TESTS_File2() {
    TEST1_File2();
    TEST2_File2();
    TEST3_File2();
    TEST4_File2();
    printf("File2 tests passed!\n");
}

int main() {
    TESTS_File1();
    TESTS_File2();

    // File1 
    File1* f1 = file1Vide();
    entree1(3, f1); entree1(5, f1);
    entree1(9, f1); entree1(7, f1);
    afficheFile1(f1);
    int x;
    sortie1(&x, f1);
    printf("x = %d\n", x);
    afficheFile1(f1); 
    printf("Size = %d\n", sizeFile1(f1));
    videFile1(f1);

    printf("\n");
    // File 2
    File2* f2 = file2Vide();
    entree2(3, f2); entree2(5, f2);
    entree2(9, f2); entree2(7, f2);
    afficheFile2(f2);
    int y;
    sortie2(&y, f2);
    printf("y = %d\n", x);
    afficheFile2(f2);
    printf("Size = %d\n", sizeFile2(f2));
    videFile2(f2);    

    return 0;
}


