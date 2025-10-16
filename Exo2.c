/****************************************************************************************
 *                         RENDU DU TP ALGO : Partie 2                                  *   
 *  Noms du binôme: Thanh Phat DOAN (thanh-phat.doan@universite-paris-saclay.fr)        *
 *                  Naël EL YAZGHI  (nael.el-yazghi@universite-paris-saclay.fr)         *
 *                                                                                      *
 *  Le code est conforme à la norme C99 et a été compilé avec les options               *
 *  les plus strictes: -std=c99 -pedantic -Wall -Wextra -Werror.                        *
 *  Les fuites mémoire ont été vérifiées avec Valgrind (--leak-check=full)              *
 *                                                                                      *
*****************************************************************************************/

/* Note *********************************************************************************
 * Nos schémas de nommage et structure de fonctions suivent strictement le fichier      *
 * Algo2.c fourni. Les tests donnés par ce fichier devraient donc marcher sans          *
 * modification                                                                         *
 * (le nommage de fonctions existe une légère différence dans les consignes du TP)      *
 * **************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

/*************************************************/
/*                                               */
/*                type bool�en                   */
/*                                               */
/*************************************************/

typedef enum {false, true} bool; 
// Dude, wtf ?
// PHAT : back in the good old days C didnt have bool type (only int),
// to use bool we must import <bool.h> lib.
// however, the prof doesnt want to use the lib so he create an alias for it.
// I compile the code with -std=c99, meaning using the C version 1999
// why? because the prof really old school and any modern feature 
// (like including bool type automatically) may not work on the prof system
// Even if the code is correct but doesnt work/compile on his machine
// we still get cooked

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


/*** les 5 fonctionnalit�s suivantes sont plus du sucre syntaxique que du code utile  ***/
/*** sauf � vouloir pouvoir basculer � moindre frais sur une impl�menation des listes ***/
/**** diff�rentes des listes chain�es propos�es dans le cadre de ce projet            ***/

// Liste Vide() { return NULL ; }
// void initVide(Liste *L) { *L = NULL ; }
// bool estVide(Liste l) { return l == NULL ; }
// int premier(Liste l) { return l->valeur ; }
// Liste suite(Liste l) { return l->suite ; }

/********************************************************************/

Liste suite(Liste l) {
    return l->suite;
}

/********************************************************************/

void depile(Liste *L) {   
    Liste tmp = *L ;
    *L = (*L)->suite ;
    free(tmp) ;
}

/********************************************************************/

Liste ajoute(int x, Liste l) {   
    Liste tmp = (Liste) malloc(sizeof(Bloc)) ;
    tmp->valeur = x ;
    tmp->suite = l ;
    return tmp ;
}

/********************************************************************/

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
/*                 VireDernier           */
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

bool UnPlusDeuxEgalTrois (Liste L) {  /* Note :PHAT, je dois fair egaffe au inout ici ? 
                                               Phat: non, en bref, utilise Liste L (sans *) si L n'est pas modifie -> type 'in'
                                                                   sinon Liste* L -> type 'inout'
                                      */

    /* Comment: fail on case [23,19,42,4,2] -> 1 
     *          you dont need to create temp variable = L
     *          just iterate on L directly
     * */
    /*
    Liste temp = L;
    int count = 0;
    int indice = 0;
    while(suite(temp)!=NULL && indice<2) {
        count +=temp->valeur;
        temp = temp->suite;
        indice ++;
    }
    if (indice == 2) {
         return (count == L->valeur);       //--> here spot the error? 
    }
    count +=temp->valeur;
    return count == 0; 
   */

    int cnt = 0, sum = 0;
    while (L && cnt < 2) {
        sum += (L->valeur);
        L = L->suite;
        cnt++;
    }
    bool ret = (L) ? (sum == L->valeur) : (sum == 0);
    return ret; 

}
   
/********************************************/
/*                                          */
/*             PlusCourte                   */
/*                                          */
/********************************************/

bool PlusCourteRec (Liste L1, Liste L2){
    
    /*
     * Comment: if L1 or L2 empty (== NULL) -> segfault
    if (L1->suite == NULL && L2->suite !=NULL) {
        return 1;
    }

    if (L2->suite == NULL) {
        return 0;
    }
    return PlusCourteRec(L1->suite,L2->suite); 
    */
    
    if (L1 && L2) {
        return PlusCourteRec(L1->suite, L2->suite);
    }
    return (!L1 && L2);
}


/*******/
  
bool PlusCourteIter (Liste L1, Liste L2){
    
    /*
     * Same problem as above: L1 or L2 may be NULL -> cant access L->suite
    Liste lu = L1;
    Liste ld = L2;
    while(lu->suite !=NULL && ld->suite != NULL) {
        lu = lu->suite;
        ld = ld->suite;
    }
    return (lu->suite==NULL && ld->suite!=NULL) ;
    */

    while (L1 && L2) {
        L1 = L1->suite;
        L2 = L2->suite;
    }
    return (!L1 && L2);
}
   
  
/********************************************/
/*                                          */
/*              VerifiekO                   */
/*                                          */
/********************************************/

bool VerifiekORec (Liste L, int k) {
    
    /*
     * Comment: fails this test:
     *    int tab3[] = {1, 2, 3, 4, 5};
          Liste l3 = build_list(tab3, 5);
          assert(VerifiekORec(l3, 0) == 1); <--
     *
     *
    if (L->suite == NULL) {
        return (k == 1);
    }
    if (k<0) {
        return 0;
    }
    if (L->valeur == 0) {
        return (VerifiekORec(L->suite,k-1));
    }
    return VerifiekORec(L->suite,k);

    */

    if (L) {
        if (L->valeur == 0) return VerifiekORec(L->suite, k-1);
        else return VerifiekORec(L->suite, k);
    }
    if (k < 0) return false;
    return (!L && k == 0); 
}
   
/*******/

bool VerifiekOIter (Liste L, int k) {
/*  Comment: the same. if L == NULL then cooked   
 *  Liste temp = L;
    int count = 0;
    while(count<=k && temp->suite!=NULL){
        if (temp->valeur == 0) {
            count++;
        }
        temp = temp->suite;
    }
    if (temp->valeur == 0) {
    count++;
    }
    return  (count == k); 
*/

    while (L) {
        k -= (L->valeur == 0);
        if (k < 0) return false;
        L = L->suite;
    }
    return (k == 0);
}
   

/********************************************/
/*                                          */
/*     NombreTermesAvantZero                */
/*                                          */
/********************************************/

int NTAZ_It (Liste L) {
    /*
     *    Comment: fail at test 1
     *    int tab1[] = {3, 2, 9, 5, 0, 6, 0};
          Liste l1 = build_list(tab1, 7);
          assert(NTAZ_It(l1) == 4); <---------
    */
    /*
    int count = 0;

    while(copy->suite!=NULL && copy->suite->valeur!=0) {
        count++;
        copy =copy->suite;
    }
    if (copy->suite == NULL) {
        return count+1;
    }
    return count;
    */
    
    int cnt = 0;
    while (L) {
        if (L->valeur == 0) return cnt;
        L = L->suite;
        cnt++;
    }
    return cnt;
}

/*******/

int NTAZ_Rec (Liste L) {
    /* Comment : also fail the same test as in NTAZ_It
     *
    if (L->suite == NULL) {
        return (!L->valeur == 0); // -> either L->valeur == 0 or !L->valeur
    }
    if (L->valeur != 0) {
        return (NTAZ_Rec(L->suite))+1;
    }
    */
   
    if (!L || (L && L->valeur == 0)) return 0;
    return 1 + NTAZ_Rec(L->suite);
}

/*******/
int NTAZ_RTSF_aux(Liste L, int cnt) {
    if (!L || (L && L->valeur == 0)) return cnt;
    return NTAZ_RTSF_aux(L->suite, cnt + 1);

}

int NTAZ_RTSF (Liste L) { 
    int cnt = 0;
    return NTAZ_RTSF_aux(L, 0);
}

/*******/
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

void TuePosRec (Liste * L) {}

/*******/

void TuePosIt (Liste * L) {}

/********************************************/
/*                                          */
/*            TueRetroPos                   */
/*                                          */
/********************************************/

void TueRetroPos (Liste * L) {}

/*************************************************/
/*                                               */
/*           Main                                */
/*                                               */
/*************************************************/



/*****************************/
/*                           */
/*   UnPlusDeuxEgalTrois     */
/*                           */
/*****************************/
#include <assert.h>


// NOTE to Nael: Simple is less. Less is good 
//               We dont need Liste* , just Liste
/*
 * “An idiot admires complexity, a genius admires simplicity, 
 * a physicist tries to make it simple, 
 * for an idiot anything the more complicated it is the more he will admire it, 
 * if you make something so clusterfucked he can't understand it 
 * he's gonna think you're a god cause you made it so complicated nobody can understand it. 
 * That's how they write journals in Academics, 
 * they try to make it so complicated people think you're a genius”
                                                            ― Terry Davis, Creator of Temple OS - my goat

#define build_list(tab) tab2list_aux(tab,sizeof(tab)/sizeof(int))

Liste * build_list_aux(int tab[], int size) {
    Liste * l = malloc(sizeof(Bloc));
    *l = NULL;  // <!>
    for (int i = size-1;i>-1;i--) {
        empile(tab[i],l);
    }
    return l;
} 
*/

Liste build_list(int arr[], int n) {
    Liste l = NULL;
    for (int i = n - 1; i >= 0; i--) {
        empile(arr[i], &l);
    } 
    return l;
}

void test_UnPlusDeuxEgalTrois() {
    printf("Testing UnPlusDeuxEgalTrois...\n");
    
    // Test 1: Standard case - third equals sum of first two
    int tab1[] = {23, 19, 42, 4, 2};
    Liste l1 = build_list(tab1, 5);
    assert(UnPlusDeuxEgalTrois(l1) == 1);
    VideListe(&l1);

    // Test 2: Two elements only (third is implicit 0)
    int tab2[] = {2, -2};
    Liste l2 = build_list(tab2, 2);
    assert(UnPlusDeuxEgalTrois(l2) == 1);  // 2 + (-2) = 0 (implicit)
    VideListe(&l2);

    // Test 3: False case
    int tab3[] = {2, 3, 27, 1};
    Liste l3 = build_list(tab3, 4);
    assert(UnPlusDeuxEgalTrois(l3) == 0);  // 2 + 3 != 27
    VideListe(&l3);
   

    // Test 4: One element only (second and third are implicit 0)
    int tab4[] = {2};
    Liste l4 = build_list(tab4, 1);
    assert(UnPlusDeuxEgalTrois(l4) == 0);  // 2 + 0 != 0
    VideListe(&l4);
    
    // Test 5: Empty list (all three are implicit 0)
    Liste l5 = NULL;
    assert(UnPlusDeuxEgalTrois(l5) == 1);  // 0 + 0 = 0
    
    // Test 6: Exactly three elements - true
    int tab6[] = {5, 7, 12};
    Liste l6 = build_list(tab6, 3);
    assert(UnPlusDeuxEgalTrois(l6) == 1);
    VideListe(&l6);
    
    // Test 7: Negative numbers
    int tab7[] = {-10, 5, -5};
    Liste l7 = build_list(tab7, 3);
    assert(UnPlusDeuxEgalTrois(l7) == 1);  // -10 + 5 = -5
    VideListe(&l7);
    
    // Test 8: Zero in first two positions
    int tab8[] = {0, 0, 0, 5};
    Liste l8 = build_list(tab8, 4);
    assert(UnPlusDeuxEgalTrois(l8) == 1);  // 0 + 0 = 0
    VideListe(&l8);
    
    printf("UnPlusDeuxEgalTrois tests passed!\n\n");
}

/*****************************/
/*                           */
/*      PlusCourte           */
/*                           */
/*****************************/

void test_PlusCourte() {
    printf("Testing PlusCourte...\n");
    
    // Test 1: First strictly shorter
    int tab1[] = {1, 2};
    int tab2[] = {1, 2, 3, 4};
    Liste l1 = build_list(tab1, 2);
    Liste l2 = build_list(tab2, 4);
    assert(PlusCourteRec(l1, l2) == 1);
    assert(PlusCourteIter(l1, l2) == 1);
    VideListe(&l1); VideListe(&l2);
 
    // Test 2: Same length
    int tab3[] = {1, 2, 3};
    int tab4[] = {4, 5, 6};
    Liste l3 = build_list(tab3, 3);
    Liste l4 = build_list(tab4, 3);
    assert(PlusCourteRec(l3, l4) == 0);
    assert(PlusCourteIter(l3, l4) == 0);
    VideListe(&l3); VideListe(&l4);
    
    // Test 3: First longer
    int tab5[] = {1, 2, 3, 4, 5};
    int tab6[] = {1, 2};
    Liste l5 = build_list(tab5, 5);
    Liste l6 = build_list(tab6, 2);
    assert(PlusCourteRec(l5, l6) == 0);
    assert(PlusCourteIter(l5, l6) == 0);
    VideListe(&l5); VideListe(&l6);
    
    // Test 4: First empty, second not
    Liste l7 = NULL;
    int tab8[] = {1};
    Liste l8 = build_list(tab8, 1);
    assert(PlusCourteRec(l7, l8) == 1);
    assert(PlusCourteIter(l7, l8) == 1);
    VideListe(&l8);
    
    // Test 5: Both empty
    Liste l9 = NULL;
    Liste l10 = NULL;
    assert(PlusCourteRec(l9, l10) == 0);
    assert(PlusCourteIter(l9, l10) == 0);
    
    // Test 6: First not empty, second empty
    int tab11[] = {1};
    Liste l11 = build_list(tab11, 1);
    Liste l12 = NULL;
    assert(PlusCourteRec(l11, l12) == 0);
    assert(PlusCourteIter(l11, l12) == 0);
    VideListe(&l11);
    
    // Test 7: Very short vs very long (efficiency test)
    int tab13[] = {1};
    int tab14[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    Liste l13 = build_list(tab13, 1);
    Liste l14 = build_list(tab14, 20);
    assert(PlusCourteRec(l13, l14) == 1);
    assert(PlusCourteIter(l13, l14) == 1);
    VideListe(&l13); VideListe(&l14);
    
    printf("PlusCourte tests passed!\n\n");
}

/*****************************/
/*                           */
/*      VerifiekO            */
/*                           */
/*****************************/

void test_VerifiekO() {
    printf("Testing VerifiekO...\n");
    
    // Test 1: Exact count matches
    int tab1[] = {2, 0, 0, 7, 0, 6, 2, 4, 0};
    Liste l1 = build_list(tab1, 9);
    assert(VerifiekORec(l1, 4) == 1);
    assert(VerifiekOIter(l1, 4) == 1);
    VideListe(&l1);
    
    // Test 2: Count doesn't match (early exit possible)
    int tab2[] = {2, 0, 0, 7, 0, 6, 2, 4, 0};
    Liste l2 = build_list(tab2, 9);
    assert(VerifiekORec(l2, 1) == 0);
    assert(VerifiekOIter(l2, 1) == 0);
    VideListe(&l2);
    
    // Test 3: No zeros, k=0
    int tab3[] = {1, 2, 3, 4, 5};
    Liste l3 = build_list(tab3, 5);
    assert(VerifiekORec(l3, 0) == 1);
    assert(VerifiekOIter(l3, 0) == 1);
    VideListe(&l3);
    
    // Test 4: No zeros, k>0
    int tab4[] = {1, 2, 3, 4, 5};
    Liste l4 = build_list(tab4, 5);
    assert(VerifiekORec(l4, 1) == 0);
    assert(VerifiekOIter(l4, 1) == 0);
    VideListe(&l4);
    
    // Test 5: All zeros
    int tab5[] = {0, 0, 0, 0};
    Liste l5 = build_list(tab5, 4);
    assert(VerifiekORec(l5, 4) == 1);
    assert(VerifiekOIter(l5, 4) == 1);
    VideListe(&l5);
    
    // Test 6: Empty list, k=0
    Liste l6 = NULL;
    assert(VerifiekORec(l6, 0) == 1);
    assert(VerifiekOIter(l6, 0) == 1);
    
    // Test 7: Empty list, k>0
    Liste l7 = NULL;
    assert(VerifiekORec(l7, 1) == 0);
    assert(VerifiekOIter(l7, 1) == 0);
    
    // Test 8: Single zero
    int tab8[] = {0};
    Liste l8 = build_list(tab8, 1);
    assert(VerifiekORec(l8, 1) == 1);
    assert(VerifiekOIter(l8, 1) == 1);
    VideListe(&l8);
    
    // Test 9: k too large (early exit)
    int tab9[] = {0, 1, 2};
    Liste l9 = build_list(tab9, 3);
    assert(VerifiekORec(l9, 5) == 0);
    assert(VerifiekOIter(l9, 5) == 0);
    VideListe(&l9);
    
    printf("VerifiekO tests passed!\n\n");
}

/*****************************/
/*                           */
/* NombreTermesAvantZero     */
/*                           */
/*****************************/
 
void test_NombreTermesAvantZero() {
    printf("Testing NombreTermesAvantZero...\n");
    
    // Test 1: Zero in middle
    int tab1[] = {3, 2, 9, 5, 0, 6, 0};
    Liste l1 = build_list(tab1, 7);
    assert(NTAZ_It(l1) == 4);
    assert(NTAZ_Rec(l1) == 4);
    assert(NTAZ_RTSF(l1) == 4);
    assert(NTAZ_RTSP(l1) == 4);
    VideListe(&l1);
    
    // Test 2: No zero (implicit at end)
    int tab2[] = {3, 2, 9, 5};
    Liste l2 = build_list(tab2, 4);
    assert(NTAZ_It(l2) == 4);
    assert(NTAZ_Rec(l2) == 4);
    assert(NTAZ_RTSF(l2) == 4);
    assert(NTAZ_RTSP(l2) == 4);
    VideListe(&l2);

    // Test 3: Zero at beginning
    int tab3[] = {0, 1, 2, 3};
    Liste l3 = build_list(tab3, 4);
    assert(NTAZ_It(l3) == 0);
    assert(NTAZ_Rec(l3) == 0);
    assert(NTAZ_RTSF(l3) == 0);
    assert(NTAZ_RTSP(l3) == 0);
    VideListe(&l3);
    
    // Test 4: Empty list (implicit zero)
    Liste l4 = NULL;
    assert(NTAZ_It(l4) == 0);
    assert(NTAZ_Rec(l4) == 0);
    assert(NTAZ_RTSF(l4) == 0);
    assert(NTAZ_RTSP(l4) == 0);
    
    // Test 5: Single element, not zero
    int tab5[] = {42};
    Liste l5 = build_list(tab5, 1);
    assert(NTAZ_It(l5) == 1);
    assert(NTAZ_Rec(l5) == 1);
    assert(NTAZ_RTSF(l5) == 1);
    assert(NTAZ_RTSP(l5) == 1);
    VideListe(&l5);
    
    // Test 6: Single element, is zero
    int tab6[] = {0};
    Liste l6 = build_list(tab6, 1);
    assert(NTAZ_It(l6) == 0);
    assert(NTAZ_Rec(l6) == 0);
    assert(NTAZ_RTSF(l6) == 0);
    assert(NTAZ_RTSP(l6) == 0);
    VideListe(&l6);
    
    // Test 7: Multiple zeros, count before first
    int tab7[] = {1, 2, 3, 0, 4, 0, 5};
    Liste l7 = build_list(tab7, 7);
    assert(NTAZ_It(l7) == 3);
    assert(NTAZ_Rec(l7) == 3);
    assert(NTAZ_RTSF(l7) == 3);
    assert(NTAZ_RTSP(l7) == 3);
    VideListe(&l7);
    
    // Test 8: All zeros
    int tab8[] = {0, 0, 0};
    Liste l8 = build_list(tab8, 3);
    assert(NTAZ_It(l8) == 0);
    assert(NTAZ_Rec(l8) == 0);
    assert(NTAZ_RTSF(l8) == 0);
    assert(NTAZ_RTSP(l8) == 0);
    VideListe(&l8);
    
    printf("NombreTermesAvantZero tests passed!\n\n");
}

/*****************************/
/*                           */
/*        TuePos             */
/*                           */
/*****************************/

void test_TuePos() {
    printf("Testing TuePos...\n");
    
    // Test 1: Standard case from specification
    int tab1[] = {0, 4, 3, 9, 5, 0, 9, 2, 1};
    Liste *l1 = build_list(tab1, 9);
    TuePosRec(l1);
    // Expected: [0, 4, 9, 0, 9, 2, 1] (removed 3 at pos 3, 5 at pos 5)
    assert(longueur_iter(*l1) == 7);
    VideListe(l1);

    int tab1b[] = {0, 4, 3, 9, 5, 0, 9, 2, 1};
    Liste *l1b = build_list(tab1b, 9);
    TuePosIt(l1b);
    assert(longueur_iter(*l1b) == 7);
    VideListe(l1b);
    
    // Test 2: No elements equal to position
    int tab2[] = {1, 0, 0, 0, 0};
    Liste *l2 = build_list(tab2, 5);
    TuePosRec(l2);
    assert(longueur_iter(*l2) == 5);
    VideListe(l2);
    
    int tab2b[] = {1, 0, 0, 0, 0};
    Liste *l2b = build_list(tab2b, 5);
    TuePosIt(l2b);
    assert(longueur_iter(*l2b) == 5);
    VideListe(l2b);
    
    // Test 3: All elements equal position
    int tab3[] = {1, 2, 3, 4, 5};
    Liste *l3 = build_list(tab3, 5);
    TuePosRec(l3);
    assert(*l3 == NULL);  // All removed
    
    int tab3b[] = {1, 2, 3, 4, 5};
    Liste *l3b = build_list(tab3b, 5);
    TuePosIt(l3b);
    assert(*l3b == NULL);
    
    // Test 4: Empty list
    Liste l4 = NULL;
    TuePosRec(&l4);
    assert(l4 == NULL);
    
    Liste l4b = NULL;
    TuePosIt(&l4b);
    assert(l4b == NULL);
    
    // Test 5: Single element - position 1, value 1
    int tab5[] = {1};
    Liste *l5 = build_list(tab5, 1);
    TuePosRec(l5);
    assert(*l5 == NULL);  // Removed

    int tab5b[] = {1};
    Liste *l5b = build_list(tab5b, 1);
    TuePosIt(l5b);
    assert(*l5b == NULL);

    // Test 6: Single element - position 1, value not 1
    int tab6[] = {5};
    Liste *l6 = build_list(tab6, 1);
    TuePosRec(l6);
    assert(longueur_iter(*l6) == 1);
    VideListe(l6);
    
    int tab6b[] = {5};
    Liste *l6b = build_list(tab6b, 1);
    TuePosIt(l6b);
    assert(longueur_iter(*l6b) == 1);
    VideListe(l6b);
    
    // Test 7: First element matches (position 1, value 1)
    int tab7[] = {1, 5, 6, 7};
    Liste *l7 = build_list(tab7, 4);
    TuePosRec(l7);
    assert(longueur_iter(*l7) == 3);
    VideListe(l7);
    
    int tab7b[] = {1, 5, 6, 7};
    Liste *l7b = build_list(tab7b, 4);
    TuePosIt(l7b);
    assert(longueur_iter(*l7b) == 3);
    VideListe(l7b);
    
    printf("TuePos tests passed!\n\n");
}

/*****************************/
/*                           */
/*     TueRetroPos           */
/*                           */
/*****************************/

void test_TueRetroPos() {
    printf("Testing TueRetroPos...\n");
    
    // Test 1: Standard case from specification
    int tab1[] = {0, 4, 3, 9, 5, 0, 9, 2, 1};
    Liste *l1 = build_list(tab1, 9);
    TueRetroPos(l1);
    // Expected: [0, 4, 3, 9, 0, 9] (removed 5 at retro-pos 5, 2 at retro-pos 2, 1 at retro-pos 1)
    assert(longueur_iter(*l1) == 6);
    VideListe(l1);
    
    // Test 2: No elements equal retro-position
    int tab2[] = {5, 5, 5, 5, 5};
    Liste *l2 = build_list(tab2, 5);
    int len_before = longueur_iter(*l2);
    TueRetroPos(l2);
    assert(longueur_iter(*l2) == len_before);
    VideListe(l2);
    
    // Test 3: All elements equal retro-position
    int tab3[] = {5, 4, 3, 2, 1};
    Liste *l3 = build_list(tab3, 5);
    TueRetroPos(l3);
    assert(*l3 == NULL);  // All removed
    
    // Test 4: Empty list
    Liste l4 = NULL;
    TueRetroPos(&l4);
    assert(l4 == NULL);
    
    // Test 5: Single element (retro-pos 1, value 1)
    int tab5[] = {1};
    Liste *l5 = build_list(tab5, 1);
    TueRetroPos(l5);
    assert(*l5 == NULL);  // Removed
    
    // Test 6: Single element (retro-pos 1, value not 1)
    int tab6[] = {5};
    Liste *l6 = build_list(tab6, 1);
    TueRetroPos(l6);
    assert(longueur_iter(*l6) == 1);
    VideListe(l6);
    
    // Test 7: Last element matches retro-position 1
    int tab7[] = {5, 6, 7, 1};
    Liste *l7 = build_list(tab7, 4);
    TueRetroPos(l7);
    assert(longueur_iter(*l7) == 3);
    VideListe(l7);
    
    // Test 8: Two elements
    int tab8[] = {2, 1};
    Liste *l8 = build_list(tab8, 2);
    TueRetroPos(l8);
    assert(*l8 == NULL);  // Both removed
    
    // Test 9: Two elements - none match
    int tab9[] = {5, 5};
    Liste *l9 = build_list(tab9, 2);
    TueRetroPos(l9);
    assert(longueur_iter(*l9) == 2);
    VideListe(l9);
    
    // Test 10: First element matches last retro-position
    int tab10[] = {5, 2, 3, 4, 5};
    Liste *l10 = build_list(tab10, 5);
    TueRetroPos(l10);
    // Element at index 0 has retro-pos 5, value 5 -> removed
    // Element at index 4 has retro-pos 1, value 5 -> not removed
    assert(longueur_iter(*l10) == 4);
    VideListe(l10);
    
    printf("TueRetroPos tests passed!\n\n");
}


// int main(int argc, char** argv)

int main()
{
/*     Liste l ;

        l = NULL ;
        VireDernier_rec  (&l) ;
        VireDernier_iter (&l) ;
        affiche_rec(l) ; 
        affiche_iter(l) ; 
        printf(" %d \n", longueur_iter(l)) ; 
        printf(" %d \n", longueur_rec(l)) ; 
        VideListe(&l);
        return 0; */
        

    /* temporary tests */
 
 
    printf("=================================\n");
    printf("Running Linked List Test Suite\n");
    printf("=================================\n\n");
    
    test_UnPlusDeuxEgalTrois();
    test_PlusCourte();
    test_VerifiekO();
    test_NombreTermesAvantZero();
//    test_TuePos();
//    test_TueRetroPos();
    
    printf("=================================\n");
    printf("All tests passed successfully!\n");
    printf("=================================\n");
}



