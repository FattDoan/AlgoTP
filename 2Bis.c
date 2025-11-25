/**************************************************************************
 *                     RENDU DU TP ALGO : Partie 2Bis                     *   
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

/*************************************************/
/*                                               */
/*                Type booléen                   */
/*                                               */
/*************************************************/

typedef enum {false, true} bool;    
// NOTE: gcc peut se plaindre si on inclut pas -std=c99

/* NOTE: ******************************************************************
 * Comme suggéré dans les consignes du TP, 
 * on utilise un compteur CptMalloc qui augmente à chaque utilisation 
 * de malloc (et diminue à chaque free). L'objectif est d'éviter 
 * les fuites de mémoire en s'assurant qu'à la fin du programme 
 * (ou plus strictement à la fin de chaque morceau de code indépendant)
 * CptMalloc == 0.
 *
 * Ici, nous définissons des fonctions wrapper de malloc et free 
 * (mallocCpt et freeCpt) qui gèrent CptMalloc afin de ne pas oublier 
 * à chaque appel malloc et free et de rendre le code plus propre. 
 **************************************************************************/

int CptMalloc = 0;  
void *mallocCpt(size_t size) {
    void *ptr = malloc(size); 
    if (ptr != NULL) CptMalloc++;
    return ptr;
}
void freeCpt(void *p) {
    if (p == NULL) return;
    assert(CptMalloc > 0);
    CptMalloc--;
    free(p);
}

/****************************************************************************/
/*                                                                          */
/*               Définition de la Liste d'entiers                           */
/*                                                                          */
/****************************************************************************/

/* NOTE: ********************************************************************
 * La définition de Bloc et Liste est la même que dans la partie 2, 
 * mais on ajoute ici un champ refCpt (compteur de références). 
 * L’objectif est de gérer proprement la mémoire lors de la 
 * compression mémoire des listes de listes.
 *
 * La fonction PPQ a une complexité exponentielle, c-a-d si on devait 
 * allouer un nouveau bloc pour chaque sous-liste générée, 
 * la mémoire exploserait rapidement.
 *
 * On remarque que les nouvelles listes partagent toujours un suffixe 
 * commun avec d’autres listes. Par ex:
 * Soit L = [[2,3], [1,4], [5]] chaque liste a une somme de q = 5 
 * et que l’on veut construire les listes de somme q+1, 
 * on ajoute 1 en tête de chaque sous-liste: L' = [[1,2,3], [1,1,4], [1,5]]
 *
 * Plutôt que de dupliquer entièrement chaque sous-liste, 
 * on peut créer 3 blocs contenant 1, dont le champ 'suite' pointe 
 * respectivement vers les têtes de [2,3], [1,4] et [5].
 * Cela permet de réutiliser les suffixes sans les recopier.
 *
 * Cependant, la partie délicate se situe lors de la libération de la mémoire
 * Suppose:   [1] <- [2] <- [4]
 *                    ^       
 *                   [3]
 *
 * Les blocs 3 et 4 pointent tous deux sur 2, qui lui-même pointe sur 1.
 * Si on libère 2 trop tôt, alors 3 et 4 pointeront vers 
 * une zone déjà libérée (double free). Même en mettant les pointeurs 
 * à NULL après libération, on ne pourrait plus remonter jusqu’à 1, 
 * ce qui provoquerait une fuite mémoire.
 *
 * Solution? Chaque Bloc possède un compteur refCpt, initialisé à 1.
 * Chaque fois qu’un bloc est référencé par une nouvelle liste, 
 * on incrémente son refCpt par 1 et lorsqu’on veut libérer un bloc, 
 * on décrémente le compteur par 1. Donc pour chaque bloc:
 *   + si refCpt > 0: autre bloc pointe encore vers lui → on ne libère pas
 *   + si refCpt == 0: on peut réellement le libérer, 
 *                     puis tenter la même opération sur son bloc suite.
 *
 * Ainsi, dans l’exemple ci-dessus :
 *   [1, ref:2] <- [2, ref: 3] <- [4, ref: 1]
 *                  ^
 *                 [3, ref: 1]
 *
 * Quel que soit l’ordre dans lequel on libère les blocs, 
 * aucun double free n’est possible. Un bloc n’est libéré 
 * que lorsque plus aucune liste ne le référence, 
 * ce qui garantit qu’aucun suffixe partagé ne sera perdu, 
 * et qu’aucun bloc ne sera libéré trop tôt. 
 *
 * **************************************************************************/
typedef struct Bloc
{
    int valeur;
    struct Bloc * suite;
    int refCpt;
} Bloc;

typedef Bloc* Liste;

/****************************************************************************/
// Fonctions élémentaires de gestion refCpt 

void incRef(Liste l) {
    if (l != NULL) l->refCpt++;
}

void decRef(Liste l) {
    if (l == NULL) return;
    l->refCpt--;
    if (l->refCpt == 0) {   
        Liste suite = l->suite;
        freeCpt(l);
        // Car ce bloc est libéré, donc faut diminue refCpt de son bloc suite 
        decRef(suite);   
    }
}

/****************************************************************************/
/* Fonction ajoute pour créer un nouveau bloc comme dans partie 2.
 * Il faut init le refCpt et augmenter refCpt son bloc suite
 */

Liste ajoute(int val, Liste suite) {
    Liste l = (Liste)mallocCpt(sizeof(Bloc));
    l->valeur = val;
    l->suite = suite;
    l->refCpt = 1;
    incRef(l->suite); 
    return l;
}

void empile(int x, Liste *L) { 
    *L = ajoute(x,*L) ; 
}

/****************************************************************************/
// Affichage iterative
void afficheListe(Liste L) {
    printf("[");
    while(L != NULL) {
        printf(" %d", L->valeur);
        L = L->suite;
        if (L != NULL) printf(",");
    }
    printf("]");
}

/****************************************************************************/
// Ici, on n'a besoin d'appeler decRef une seule fois et il s'occupera
// car on ne peut que réellement libérer les blocs si refCnt == 0
void videListe(Liste *L) {
    if (*L == NULL) return; 
    decRef(*L);
    *L = NULL;
}

/****************************************************************************/
/*                                                                          */
/*               Définition de la Liste des Listes d'entiers                */
/*                                                                          */
/****************************************************************************/
// L'idée est la même, chaque 'élément' est de type Liste au lieu de int

typedef struct BlocListe {
    Liste l; 
    struct BlocListe * suite;
} BlocListe;

typedef BlocListe* ListeListe;

/****************************************************************************/
// Affichage iterative
// Pour chaque liste, on appele afficheListe
void afficheListeListe(ListeListe L) {
    printf("[");
    while(L != NULL) {
        afficheListe(L->l);
        L = L->suite;
        if (L != NULL) printf(", ");
    }
    printf("]\n");        
}

/****************************************************************************/
// Car decRef s'occupe au niveau de la liste, donc il faut parcourir pour
// chaque Liste de ListeListe on l'appele.

void videListeListe(ListeListe *L) {   
    while (*L != NULL) {
        ListeListe tmp = *L;
        *L = tmp->suite;
        decRef(tmp->l);
        
        // Les blocs de la Liste peuvent toujours être encore référencés, 
        // mais on peut libérer la mémoire (ListeListe) contenant 
        // les pointeurs vers ces blocs 
        freeCpt(tmp);
    } 
}

/****************************************************************************/
// Concaténer deux listes de listes : L = L1 @ L2
// Ex: concat( [[2,3], [1]] , [[1,2], [3]]) donne [[2,3], [1], [1,2,3], [3]]
// L'idée est de lier la fin de L1 au début de L2. Complexité: O( |L1| )
ListeListe concat(ListeListe L1, ListeListe L2) {
    if (L1 == NULL) return L2;
    
    ListeListe L = L1;
    while (L->suite != NULL) L = L->suite;
    L->suite = L2;
    
    return L1;  
}

/****************************************************************************/
// Ajout d'un élément x en tête de toutes les listes de ListeListe
// Ex: addToAll(x, [[2, 3], [1], [4]]) donne [[x, 2, 3], [x, 1], [x, 4]]
// L'idée: pour chaque liste, un nouveau bloc de x pointe vers sa tête 
// Complexité: O( |L| )
ListeListe addToAll(int x, ListeListe L) {
    if (L == NULL) return NULL;
    ListeListe res = NULL;
    ListeListe* ptr = &res;

    while (L != NULL) { 
        Liste newHead = ajoute(x, L->l); 
       
        // a new BlocListe
        *ptr = (ListeListe)mallocCpt(sizeof(BlocListe));
        (*ptr)->l = newHead;
        (*ptr)->suite = NULL;
        ptr = &((*ptr)->suite);

        L = L->suite;

    }
    return res;
}

/**************************************************************************/
/*                                                                        */
/*                                   PPQ                                  */
/*                                                                        */
/**************************************************************************/
/* NOTE : *****************************************************************
 * A l'aide des consignes du TP, on déduit une formule récursive:
 * PPQ(p1, p2, q) = 
 * | q == 0 ->  [[]]                          
 * | q < p1 ->  []      
 * | _      ->  Concaténer, pour chaque p dans [p1, p2] tel que p ≤ q,
 *              le résultat de addToAll(p, PPQ(p1, p2, q - p)) 
 * 
 * On observe que q1 et q2 sont constantes donc il y a au maxi 
 * q sous-problèmes pour n'importe appel de PPQ(p1, p2, q) ~ PPQ(q). 
 * On peut donc optimiser la récursion en mémoriser les résulat de 
 * sous-problèmes. C-a-d on stocke le résulat de mem[q] = PPQ(q) 
 * et pour tout q' = q + p où p dans [p1,p2],
 * on peut calculer PPQ(q') sans recalculer PPQ(q).
 * C'est recursion with memoization.
 *
 * On définit une fonction auxiliaire rec_PPQ pour stocker mem qui est 
 * un tableau d'adresses de BlocListe (ListeListe).  
 * Chaque fois que l'on obtient le résultat d'un sous-problème q, 
 * on l'attribue à mem[q] et on n'a plus qu'à le renvoyer 
 * pour les appels à PPQ(p1, p2, q).
 *
 * Finalement, dans la fonction PPQ, on initialise mem et désalloue la
 * mémoire avant de sortir.
 *
 * *************************************************************************/

ListeListe rec_PPQ(int p1, int p2, int q, ListeListe* mem) { 
    if (mem[q] != NULL) return mem[q];

    if (q < p1) return NULL;
    
    ListeListe res = NULL;
    for (int p = p1; p <= p2; p++) {        
        if (q >= p) {
            res = concat(res, addToAll(p, rec_PPQ(p1, p2, q - p, mem)));
        }
    }
    mem[q] = res;
    return res;
}
ListeListe PPQ(int p1, int p2, int q) {
    assert(q >= 0);
    
    ListeListe* mem = mallocCpt(sizeof(ListeListe) * (q+1));
    for (int i = 0; i <= q; i++) mem[i] = NULL;
    
    // pour le cas q = 0, mem[0] pointe vers [ [] ]
    mem[0] = (ListeListe)mallocCpt(sizeof(BlocListe)); 
    mem[0]->l = NULL; 
    mem[0]->suite = NULL;

    ListeListe res = rec_PPQ(p1, p2, q, mem);
    // ne laisse que mem[q], désalloue le reste
    for (int i = 0; i < q; i++) {
        videListeListe(&mem[i]);
    }
    freeCpt(mem);
    return res;
}

/********************************************************************/
// Fonctions utilitaires pour les tests

Liste build_list(int arr[], int n) {
    Liste l = NULL;
    for (int i = n - 1; i >= 0; i--) {
        empile(arr[i], &l);
    } 
    return l;
}

// Les listes attendues doivent être dans le même ordre que L
// bof mais ca marche pour tester
bool eqLL_2DArr(ListeListe L, int tab[][20], int r, int c[]) {
    for (int i = 0; i < r; i++) {
        if (L == NULL) return false;

        Liste l = L->l;
        for (int j = 0; j < c[i]; j++) {
            if (l == NULL) return false;
            if (l->valeur != tab[i][j]) return false;
            l = l->suite;
        }
        if (l != NULL) return false;

        L = L->suite;
    }
    return L == NULL;
}

void TEST_BASE_CASE() {
    ListeListe L = PPQ(2, 4, 0);
    assert(L != NULL);
    assert(L->l == NULL);
    assert(L->suite == NULL);
    videListeListe(&L);
}
void TEST_P_LT_Q1() {
    ListeListe L = PPQ(3, 5, 2);
    assert(L == NULL);
}

void TEST_1_3_4() {
    ListeListe L = PPQ(1,4,4);

    int tab[8][20] = {
        {1,1,1,1}, {1,1,2}, {1,2,1}, {1,3}, {2,1,1}, {2,2}, {3,1}, {4}
    };
    int cols[8] = {4, 3, 3, 2, 3, 2, 2, 1};

    assert(eqLL_2DArr(L, tab, 8, cols));

    videListeListe(&L);
    assert(CptMalloc == 0);
}
void TEST_2_5_6() {
    ListeListe L = PPQ(2,5,6);

    int tab[4][20] = {{2,2,2}, {2,4}, {3,3}, {4,2}};
    int cols[4] = {3, 2, 2, 2};

    assert(eqLL_2DArr(L, tab, 4, cols));

    videListeListe(&L);
    assert(CptMalloc == 0);
}
void TEST_2_4_9() {
    ListeListe L = PPQ(2,4,9);

    int tab[11][20] = {
        {2, 2, 2, 3}, {2, 2, 3, 2}, {2, 3, 2, 2}, {2, 3, 4}, {2, 4, 3}, 
        {3, 2, 2, 2}, {3, 2, 4}, {3, 3, 3}, {3, 4, 2}, 
        {4, 2, 3}, {4, 3, 2}
    };
    int cols[11] = {4, 4, 4, 3, 3, 4, 3, 3, 3, 3, 3};

    assert(eqLL_2DArr(L, tab, 11, cols));

    videListeListe(&L);
    assert(CptMalloc == 0);
}

void TESTS() {
    TEST_BASE_CASE();
    TEST_P_LT_Q1();
    TEST_1_3_4();
    TEST_2_5_6();
    TEST_2_4_9();
}
int main() {
    TESTS();

    ListeListe L = PPQ(2,4,9);
    afficheListeListe(L);
    videListeListe(&L);
    assert(CptMalloc == 0); 
    return 0;
}


