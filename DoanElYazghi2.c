/**************************************************************************
 *                     RENDU DU TP ALGO : Partie 2                        *   
 *  Noms du binôme:                                                       *
 *  - Thanh Phat DOAN (thanh-phat.doan@universite-paris-saclay.fr)        *
 *  - Naël EL YAZGHI  (nael.el-yazghi@universite-paris-saclay.fr)         *
 *                                                                        *               
 *  Le code est conforme à la norme C99 et a été compilé avec les options *               
 *  les plus strictes: -std=c99 -pedantic -Wall -Wextra -Werror.          *
 *  Les fuites mémoire ont été vérifiées avec Valgrind (--leak-check=full)*
 *                                                                        *
***************************************************************************/

/* Note *******************************************************************
 * Nos schémas de nommage et structure de fonctions suivent strictement   *
 * le fichier Algo2.c fourni. Les tests donnés par ce fichier devraient   *
 * donc marcher sans modification (le nommage de fonctions existe une     *
 * légère différence dans les consignes du TP)                            *
 * ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

/*************************************************/
/*                                               */
/*                type bool�en                   */
/*                                               */
/*************************************************/

typedef enum {false, true} bool;    
// NOTE: gcc peut se plaindre si on inclut pas -std=c99

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

typedef Bloc* Liste;

/*************************************************/
/*                                               */
/*                briques de base                */
/*                                               */
/*************************************************/


/************* Sucre syntaxique ******************/
/* Ce sont des fonctions données dans le TD */

Liste Vide() { return NULL ; }
void initVide(Liste *L) { *L = NULL ; }
bool estVide(Liste l) { return l == NULL ; }
int premier(Liste l) { return l->valeur ; }

Liste suite(Liste l) {
    return l->suite;
}

void depile(Liste *L) {   
    Liste tmp = *L ;
    *L = (*L)->suite ;
    free(tmp) ;
}

Liste ajoute(int x, Liste l) {   
    Liste tmp = (Liste) malloc(sizeof(Bloc)) ;
    tmp->valeur = x ;
    tmp->suite = l ;
    return tmp ;
}

void empile(int x, Liste *L) { 
    *L = ajoute(x,*L) ; 
}

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

/********************************************************************/

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

/********************************************************************/


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
/*          VireDernier                  */
/*     avec un depile                    */
/* � la main opportuniste (version iter) */
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

/********************************************************************/

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

/*
 *  On ajoute L->valeur et (L->suite)->valeur à sum si possible
 *  --> de toute facon, si |L| < 2, 
 *  les nombres manquants sont remplacés par des zéros, 
 *  conformément à l'énoncé.
 *  
 *  A la fin, L est sur le 3eme element,  si (L == NULL), 
 *  cad |L| < 3, on compare avec 0, sinon,
 *  on compare avec le 3e élément. 
 *
 */

bool UnPlusDeuxEgalTrois (Liste L) { 
    int sum = 0;
    if (L != NULL) {
        sum += (L->valeur);
        if (L->suite) {
            L = L->suite;
            sum += (L->valeur);
        }
    }

    bool ret = (L != NULL) ? (sum == L->valeur) : (sum == 0);
    return ret; 
}
   
/********************************************/
/*                                          */
/*             PlusCourte                   */
/*                                          */
/********************************************/


/*
 * On parcourt les 2 listes L1 et L2 en même temps
 * Si L1 == NULL avant L2, alors |L1| < |L2|
 * Sinon, |L1| >= |L2|
 */

bool PlusCourteRec (Liste L1, Liste L2){
    if (L1 != NULL && L2 != NULL) {
        return PlusCourteRec(L1->suite, L2->suite);
    }
    return (L1 == NULL && L2 != NULL);
}

/********************************************/
  
bool PlusCourteIter (Liste L1, Liste L2){
    while (L1 != NULL && L2 != NULL) {
        L1 = L1->suite;
        L2 = L2->suite;
    }
    return (L1 == NULL && L2 != NULL);
}
    
/********************************************/
/*                                          */
/*              VerifiekO                   */
/*                                          */
/********************************************/

/*
 *  On utilise la variable k pour compter le nombre de 0 
 *  que il faut on doit encore renconter (si on voit un 0, on diminue k). 
 *  Notre objectif est que k soit exactement égal à 0 quand L == NULL
 *  (cad on a vu exactment k 0s pendant le parcours).
 *
 *  La formule de récursion est donc:
 *
 *  Verifiek0(L, k) = Verifiek0(L->suite, k - 1) si L->valeur == 0
 *                  = Verifiek0(L->suite, k)     si L->valeur != 0
 *  Cas de base:
 *          si k < 0 -> false   (on arrête dès que possible)
 *          si L == NULL        (on a parcouru jusqu'a` la fin) 
 *             --> renvoie (k == 0)
 */

bool VerifiekORec (Liste L, int k) {
    if (L != NULL) {
        if (L->valeur == 0) return VerifiekORec(L->suite, k-1);
        else return VerifiekORec(L->suite, k);
    }
    if (k < 0) return false;
    return (L == NULL && k == 0); 
}
   
/********************************************************************/

bool VerifiekOIter (Liste L, int k) {
    while (L != NULL && k >= 0) {
        k -= (L->valeur == 0);
        L = L->suite;
    }
    return (k == 0);
}
   

/********************************************/
/*                                          */
/*         NombreTermesAvantZero            */
/*                                          */
/********************************************/

/************ Version récursive *************/
/*
 *  Le code itératif est assez intuitif.
 *  On parcourt jusqu'à ce que l'on rencontre le 1er 0
 *  -> On renvoie tout de suite la position de ce dernier.
 *
 */

int NTAZ_It (Liste L) {    
    int cnt = 0;
    while (L != NULL && L->valeur != 0) {
        L = L->suite;
        cnt++;
    }
    return cnt;
}

/******* Version récursive sans sous-fonctionalité (non terminale) *****/
/*
 *  Formule de recursion:
 *  if (L != NULL):
 *      NTAZ(L) = 1 + NTAZ(L->suite) si L->valeur != 0    
 *      NTAZ(L) = 0                  si L->valeur == 0  --> Cas de base
 *  else:
 *      NTAZ(L) = 0                                     --> Cas de base
 */         

int NTAZ_Rec (Liste L) {   
    if (L == NULL || (L != NULL && L->valeur == 0)) return 0;
    return 1 + NTAZ_Rec(L->suite);
}

/******* Version avec une sous-fonction récursive terminale *****/
/*
 *  Pour être récursif terminal, on stocke le résultat dans 
 *  une valeur cnt qui est passée en argument (le compteur "in") 
 *  et on la renvoie lors du cas de base
 *  On réécrit donc la formule de cette manière:
 *  if (L != NULL):
 *      NTAZ(L, cnt) = NTAZ(L->suite, cnt+1 ) si L->valeur != 0    
 *      NTAZ(L, cnt) = cnt                    si L->valeur == 0 -> Cas de base
 *  else:       
 *      NTAZ(L, cnt) = cnt                                      -> Cas de base
 *  
 *  On commence par NTAZ(L, 0) où L est la liste d'origine
 *
 */  

int NTAZ_RTSF_aux(Liste L, int cnt) {
    if (L == NULL || (L != NULL && L->valeur == 0)) return cnt;
    return NTAZ_RTSF_aux(L->suite, cnt + 1);

}

int NTAZ_RTSF (Liste L) { 
    return NTAZ_RTSF_aux(L, 0);
}

/******* Version avec une sous-procédure récursive terminale *****/
/*
 * On ne peut pas renvoyer le resultat, on met donc à jour le resultat
 * directment via un pointeur d'entier (int* cnt : le compteur "inout"). 
 */

void NTAZ_RTSP_aux(Liste L, int* cnt) {
    if (!L || (L && L->valeur == 0)) return;
    (*cnt)++;
    NTAZ_RTSP_aux(L->suite, cnt);
}

int NTAZ_RTSP (Liste L) { 
    int cnt = 0;
    NTAZ_RTSP_aux(L, &cnt);
    return cnt;
}


/********************************************/
/*                                          */
/*              TuePos                      */
/*                                          */
/********************************************/

/************* Version recursive ************/
/*
 * Comme on a besoin de connaître à chaque etape la position courante,
 * on definit donc une procedure auxiliaire permet d'initialiser pos à 1
 *
 * Il faut ensuite être dans le cas où l'élément actuel == pos :
 * après la suppression de cet élément, on ne doit pas avancer encore pointeur L.
 */

void TuePosRec_aux(Liste *L, int pos) {
    if (*L && (*L)->valeur == pos) {
        depile(L);
        TuePosRec_aux(L, pos + 1);
    }
    else if (*L) {
        L = &((*L)->suite);
        TuePosRec_aux(L, pos + 1);
    }
}

void TuePosRec (Liste * L) {
    TuePosRec_aux(L, 1);
}

/************* Version iterative ************/
void TuePosIt (Liste * L) {
    int pos = 1;
    while (*L) {  
        if ((*L)->valeur == pos) depile(L);
        else L = &((*L)->suite);

        pos++;
    }
}

/********************************************/
/*                                          */
/*            TueRetroPos                   */
/*                                          */
/********************************************/

/*
 * L'atuce est inspirée par question 11. Pif(l) du TD2.
 * 
 * On maintient un pointer posBack
 * (moralement c'est une variable "globale" pour cette fonction)
 * On avance L par un appel récursif pour arriver à la fin de la liste. 
 * Lorsque l'on depile, on augmente posBack -> 
 * la position courante en partant de la fin
 * Donc, si l'élement courant a pour valeur posBack, on l'enlève 
 *
 */
void TueRetroPos_aux(Liste* L, int* posBack) {
    if (*L) {
        TueRetroPos_aux(&((*L)->suite), posBack);
        (*posBack)++;
        if ((*posBack) == (*L)->valeur) {
            depile(L);
        }
    }

}

void TueRetroPos (Liste * L) {
    int posBack = 0;
    TueRetroPos_aux(L, &posBack);
}

/**********************************************************/
/*                                                        */
/*               Suite de tests automatiques              */
/*                                                        */
/**********************************************************/
#include <assert.h>

/* helper functions pour faire des tests */
Liste build_list(int arr[], int n) {
    Liste l = NULL;
    for (int i = n - 1; i >= 0; i--) {
        empile(arr[i], &l);
    } 
    return l;
}
bool check_equals(Liste l, int arr[], int n) {
    int i = 0;
    while (l) {
        if (l->valeur != arr[i]) return false;
        l = l->suite;
        i++;
    }
    return (i == n && !l);
}
/**********************************************************/
/*                                                        */
/*               TEST UnPlusDeuxEgalTrois                 */
/*                                                        */
/**********************************************************/

void test_UnPlusDeuxEgalTrois(void) {
    printf("Testing UnPlusDeuxEgalTrois...\n");
    
    // Test 1
    int tab1[] = {23, 19, 42, 4, 2};
    Liste l1 = build_list(tab1, 5);
    assert(UnPlusDeuxEgalTrois(l1) == 1);
    VideListe(&l1);

    // Test 2
    int tab2[] = {2, -2};
    Liste l2 = build_list(tab2, 2);
    assert(UnPlusDeuxEgalTrois(l2) == 1);  // 2 + -2 = 0
    VideListe(&l2);

    // Test 3
    int tab3[] = {2, 3, 27, 1};
    Liste l3 = build_list(tab3, 4);
    assert(UnPlusDeuxEgalTrois(l3) == 0);  // 2 + 3 != 27
    VideListe(&l3);
   

    // Test 4
    int tab4[] = {2};
    Liste l4 = build_list(tab4, 1);
    assert(UnPlusDeuxEgalTrois(l4) == 0);  // 2 + 0 != 0
    VideListe(&l4);
    
    // Test 5: Liste vide
    Liste l5 = NULL;
    assert(UnPlusDeuxEgalTrois(l5) == 1);  // 0 + 0 = 0
    
    // Test 6: Exactement 3 elements
    int tab6[] = {5, 7, 12};
    Liste l6 = build_list(tab6, 3);
    assert(UnPlusDeuxEgalTrois(l6) == 1);
    VideListe(&l6);
    
    // Test 7: Entiers negatifs
    int tab7[] = {-10, 5, -5};
    Liste l7 = build_list(tab7, 3);
    assert(UnPlusDeuxEgalTrois(l7) == 1);  // -10 + 5 = -5
    VideListe(&l7);
    
    // Test 8
    int tab8[] = {0, 0, 0, 5};
    Liste l8 = build_list(tab8, 4);
    assert(UnPlusDeuxEgalTrois(l8) == 1);  // 0 + 0 = 0
    VideListe(&l8);
    
    printf("UnPlusDeuxEgalTrois tests passed!\n\n");
}

/**********************************************************/
/*                                                        */
/*                  TEST PlusCourte                       */
/*                                                        */
/**********************************************************/

void test_PlusCourte(void) {
    printf("Testing PlusCourte...\n");
    
    // Test 1: |l1| < |l2|
    int tab1[] = {1, 2};
    int tab2[] = {1, 2, 3, 4};
    Liste l1 = build_list(tab1, 2);
    Liste l2 = build_list(tab2, 4);
    assert(PlusCourteRec(l1, l2) == 1);
    assert(PlusCourteIter(l1, l2) == 1);
    VideListe(&l1); VideListe(&l2);
 
    // Test 2: |l1| = |l2|
    int tab3[] = {1, 2, 3};
    int tab4[] = {4, 5, 6};
    Liste l3 = build_list(tab3, 3);
    Liste l4 = build_list(tab4, 3);
    assert(PlusCourteRec(l3, l4) == 0);
    assert(PlusCourteIter(l3, l4) == 0);
    VideListe(&l3); VideListe(&l4);
    
    // Test 3: |l1| > |l2|
    int tab5[] = {1, 2, 3, 4, 5};
    int tab6[] = {1, 2};
    Liste l5 = build_list(tab5, 5);
    Liste l6 = build_list(tab6, 2);
    assert(PlusCourteRec(l5, l6) == 0);
    assert(PlusCourteIter(l5, l6) == 0);
    VideListe(&l5); VideListe(&l6);
    
    // Test 4: |l1| = 0, |l2| > 0
    Liste l7 = NULL;
    int tab8[] = {1};
    Liste l8 = build_list(tab8, 1);
    assert(PlusCourteRec(l7, l8) == 1);
    assert(PlusCourteIter(l7, l8) == 1);
    VideListe(&l8);
    
    // Test 5: |l1| = |l2| = 0
    Liste l9 = NULL;
    Liste l10 = NULL;
    assert(PlusCourteRec(l9, l10) == 0);
    assert(PlusCourteIter(l9, l10) == 0);
    
    // Test 6: |l1| > 0, |l2| = 0
    int tab11[] = {1};
    Liste l11 = build_list(tab11, 1);
    Liste l12 = NULL;
    assert(PlusCourteRec(l11, l12) == 0);
    assert(PlusCourteIter(l11, l12) == 0);
    VideListe(&l11);
    
    // Test 7: |l1| << |l2|
    int tab13[] = {1};
    int tab14[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    Liste l13 = build_list(tab13, 1);
    Liste l14 = build_list(tab14, 20);
    assert(PlusCourteRec(l13, l14) == 1);
    assert(PlusCourteIter(l13, l14) == 1);
    VideListe(&l13); VideListe(&l14);
    
    printf("PlusCourte tests passed!\n\n");
}

/**********************************************************/
/*                                                        */
/*                     TEST Verifiek0                     */
/*                                                        */
/**********************************************************/

void test_VerifiekO(void) {
    printf("Testing VerifiekO...\n");
    
    // Test 1
    int tab1[] = {2, 0, 0, 7, 0, 6, 2, 4, 0};
    Liste l1 = build_list(tab1, 9);
    assert(VerifiekORec(l1, 4) == 1);
    assert(VerifiekOIter(l1, 4) == 1);
    VideListe(&l1);
    
    // Test 2
    int tab2[] = {2, 0, 0, 7, 0, 6, 2, 4, 0};
    Liste l2 = build_list(tab2, 9);
    assert(VerifiekORec(l2, 1) == 0);
    assert(VerifiekOIter(l2, 1) == 0);
    VideListe(&l2);
    
    // Test 3
    int tab3[] = {1, 2, 3, 4, 5};
    Liste l3 = build_list(tab3, 5);
    assert(VerifiekORec(l3, 0) == 1);
    assert(VerifiekOIter(l3, 0) == 1);
    VideListe(&l3);
    
    // Test 4
    int tab4[] = {1, 2, 3, 4, 5};
    Liste l4 = build_list(tab4, 5);
    assert(VerifiekORec(l4, 1) == 0);
    assert(VerifiekOIter(l4, 1) == 0);
    VideListe(&l4);
    
    // Test 5
    int tab5[] = {0, 0, 0, 0};
    Liste l5 = build_list(tab5, 4);
    assert(VerifiekORec(l5, 4) == 1);
    assert(VerifiekOIter(l5, 4) == 1);
    VideListe(&l5);
    
    // Test 6
    Liste l6 = NULL;
    assert(VerifiekORec(l6, 0) == 1);
    assert(VerifiekOIter(l6, 0) == 1);
    
    // Test 7
    Liste l7 = NULL;
    assert(VerifiekORec(l7, 1) == 0);
    assert(VerifiekOIter(l7, 1) == 0);
    
    // Test 8
    int tab8[] = {0};
    Liste l8 = build_list(tab8, 1);
    assert(VerifiekORec(l8, 1) == 1);
    assert(VerifiekOIter(l8, 1) == 1);
    VideListe(&l8);
    
    // Test 9
    int tab9[] = {0, 1, 2};
    Liste l9 = build_list(tab9, 3);
    assert(VerifiekORec(l9, 5) == 0);
    assert(VerifiekOIter(l9, 5) == 0);
    VideListe(&l9);
    
    printf("VerifiekO tests passed!\n\n");
}

/********************************************************/
/*                                                      */
/*                TEST NombreTermesAvantZero            */
/*                                                      */
/********************************************************/
 
void test_NombreTermesAvantZero(void) {
    printf("Testing NombreTermesAvantZero...\n");
    
    // Test 1: Zero au milieu 
    int tab1[] = {3, 2, 9, 5, 0, 6, 0};
    Liste l1 = build_list(tab1, 7);
    assert(NTAZ_It(l1) == 4);
    assert(NTAZ_Rec(l1) == 4);
    assert(NTAZ_RTSF(l1) == 4);
    assert(NTAZ_RTSP(l1) == 4);
    VideListe(&l1);
    
    // Test 2: Pas de zero
    int tab2[] = {3, 2, 9, 5};
    Liste l2 = build_list(tab2, 4);
    assert(NTAZ_It(l2) == 4);
    assert(NTAZ_Rec(l2) == 4);
    assert(NTAZ_RTSF(l2) == 4);
    assert(NTAZ_RTSP(l2) == 4);
    VideListe(&l2);

    // Test 3: Zero au debut
    int tab3[] = {0, 1, 2, 3};
    Liste l3 = build_list(tab3, 4);
    assert(NTAZ_It(l3) == 0);
    assert(NTAZ_Rec(l3) == 0);
    assert(NTAZ_RTSF(l3) == 0);
    assert(NTAZ_RTSP(l3) == 0);
    VideListe(&l3);
    
    // Test 4: Vide
    Liste l4 = NULL;
    assert(NTAZ_It(l4) == 0);
    assert(NTAZ_Rec(l4) == 0);
    assert(NTAZ_RTSF(l4) == 0);
    assert(NTAZ_RTSP(l4) == 0);
    
    // Test 5: Un seul element
    int tab5[] = {42};
    Liste l5 = build_list(tab5, 1);
    assert(NTAZ_It(l5) == 1);
    assert(NTAZ_Rec(l5) == 1);
    assert(NTAZ_RTSF(l5) == 1);
    assert(NTAZ_RTSP(l5) == 1);
    VideListe(&l5);
    
    // Test 6: Un seul element qui est zero
    int tab6[] = {0};
    Liste l6 = build_list(tab6, 1);
    assert(NTAZ_It(l6) == 0);
    assert(NTAZ_Rec(l6) == 0);
    assert(NTAZ_RTSF(l6) == 0);
    assert(NTAZ_RTSP(l6) == 0);
    VideListe(&l6);
    
    // Test 7
    int tab7[] = {1, 2, 3, 0, 4, 0, 5};
    Liste l7 = build_list(tab7, 7);
    assert(NTAZ_It(l7) == 3);
    assert(NTAZ_Rec(l7) == 3);
    assert(NTAZ_RTSF(l7) == 3);
    assert(NTAZ_RTSP(l7) == 3);
    VideListe(&l7);
    
    // Test 8
    int tab8[] = {0, 0, 0};
    Liste l8 = build_list(tab8, 3);
    assert(NTAZ_It(l8) == 0);
    assert(NTAZ_Rec(l8) == 0);
    assert(NTAZ_RTSF(l8) == 0);
    assert(NTAZ_RTSP(l8) == 0);
    VideListe(&l8);
    
    printf("NombreTermesAvantZero tests passed!\n\n");
}

/********************************************************/
/*                                                      */
/*                      TEST TuePos                     */
/*                                                      */
/********************************************************/

void test_TuePos(void) {
    printf("Testing TuePos...\n");
    
    // Test 1: Cas du consigne TP
    int tab1[] = {0, 4, 3, 9, 5, 0, 9, 2, 1};
    Liste l1 = build_list(tab1, 9);
    TuePosRec(&l1);     // Faut donner [0, 4, 9, 0, 9, 2, 1]
    int tab1_[] = {0, 4, 9, 0, 9, 2, 1}; 
    assert(check_equals(l1, tab1_, 7));
    VideListe(&l1);
    
    // Test 2: premier element = pos
    int tab2[] = {1, 0, 0, 0, 0};
    Liste l2 = build_list(tab2, 5);
    TuePosRec(&l2);     // Faut donner [0, 0, 0, 0]
    int tab2_[] = {0, 0, 0, 0}; 
    assert(check_equals(l2, tab2_, 4));
    VideListe(&l2);
        
    // Test 3
    int tab3[] = {1, 2, 3, 4, 5};
    Liste l3 = build_list(tab3, 5);
    TuePosRec(&l3);
    assert(l3 == NULL);  // Tout enleve'
    
    // Test 4: Liste vide
    Liste l4 = NULL;
    TuePosRec(&l4);
    assert(l4 == NULL);
    
    // Test 5: Un seul element 1
    int tab5[] = {1};
    Liste l5 = build_list(tab5, 1);
    TuePosRec(&l5);
    assert(l5 == NULL);  

    // Test 6: Un seul element != 1
    int tab6[] = {5};
    Liste l6 = build_list(tab6, 1);
    TuePosRec(&l6);     // Faut donner [5]
    assert(check_equals(l6, tab6, 1));
    VideListe(&l6);
     
    // Test 7
    int tab7[] = {1, 5, 3, 7};
    Liste l7 = build_list(tab7, 4);
    TuePosRec(&l7);     // Faut donner [5, 7]
    int tab7_[] = { 5, 7};
    assert(check_equals(l7, tab7_, 2));
    VideListe(&l7);
    
    printf("TuePos tests passed!\n\n");
}

/********************************************************/
/*                                                      */
/*                   TEST TueRetroPos                   */
/*                                                      */
/********************************************************/

void test_TueRetroPos(void) {
    printf("Testing TueRetroPos...\n");
    
    // Test 1: Cas du consigne
    int tab1[] = {0, 4, 3, 9, 5, 0, 9, 2, 1};
    Liste l1 = build_list(tab1, 9);
    TueRetroPos(&l1); // Faut donner [0, 4, 3, 9, 0, 9]
    int tab1_[] = {0, 4, 3, 9, 0, 9};
    assert(check_equals(l1, tab1_, 6));
    VideListe(&l1);
    
    // Test 2
    int tab2[] = {5, 5, 5, 5, 5};
    Liste l2 = build_list(tab2, 5);
    TueRetroPos(&l2); // Faut donner [5, 5, 5, 5]
    int tab2_[] = {5, 5, 5, 5};
    assert(check_equals(l2, tab2_, 4));
    VideListe(&l2);
    
    // Test 3
    int tab3[] = {5, 4, 3, 2, 1};
    Liste l3 = build_list(tab3, 5);
    TueRetroPos(&l3);
    assert(l3 == NULL);  // Tout enleve'
    
    // Test 4
    Liste l4 = NULL;
    TueRetroPos(&l4);
    assert(l4 == NULL);
    
    // Test 5
    int tab5[] = {1};
    Liste l5 = build_list(tab5, 1);
    TueRetroPos(&l5);
    assert(l5 == NULL);  
    
    // Test 6
    int tab6[] = {5};
    Liste l6 = build_list(tab6, 1);
    TueRetroPos(&l6); // Faut donner [5]
    int tab6_[] = {5};
    assert(check_equals(l6, tab6_, 1));
    VideListe(&l6);
    
    // Test 7
    int tab7[] = {5, 6, 7, 1};
    Liste l7 = build_list(tab7, 4);
    TueRetroPos(&l7);   // Faut donner [5, 6, 7]
    int tab7_[] = {5, 6, 7};
    assert(check_equals(l7, tab7_, 3));    
    VideListe(&l7);
    
    // Test 8: 2 elements
    int tab8[] = {2, 1};
    Liste l8 = build_list(tab8, 2);
    TueRetroPos(&l8);
    assert(l8 == NULL); 
    
    // Test 9
    int tab9[] = {5, 5};
    Liste l9 = build_list(tab9, 2);
    TueRetroPos(&l9);
    assert(check_equals(l9, tab9, 2));
    VideListe(&l9);
    
    // Test 10
    int tab10[] = {5, 2, 3, 4, 5};
    Liste l10 = build_list(tab10, 5);
    TueRetroPos(&l10);  // Faut donner [2, 4, 5]
    int tab10_[] = {2, 4, 5};
    assert(check_equals(l10, tab10_, 3));
    VideListe(&l10);
    
    printf("TueRetroPos tests passed!\n\n");
}

int main(void)
{
    printf("=================================\n");
    printf("Running Linked List Test Suite\n");
    printf("=================================\n\n");
    
    test_UnPlusDeuxEgalTrois();
    test_PlusCourte();
    test_VerifiekO();
    test_NombreTermesAvantZero();
    test_TuePos();
    test_TueRetroPos();
    
    printf("=================================\n");
    printf("All tests passed successfully!\n");
    printf("=================================\n");
}



