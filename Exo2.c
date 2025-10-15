/****************************************************************************************
 *                         RENDU DU TP ALGO : Partie 2                                  *   
 *  Noms du binôme: Thanh Phat DOAN (thanh-phat.doan@universite-paris-saclay.fr)        *
 *                  Naël EL YAZGHI  ()                                                  *
 *                                                                                      *
 *  Le code est conforme à la norme C99 et a été compilé avec les options               *
 *  les plus strictes: -std=c99 -pedantic -Wall -Wextra -Werror.                        *
 *  Les fuites mémoire ont été vérifiées avec Valgrind (--leak-check=full)              *
 *                                                                                      *
*****************************************************************************************/

/* Note *********************************************************************************
 * Nos schémas de nommage et structure de fonctions suivent strictement le fichier      *
 * Algo1.c fourni. Les tests donnés par ce fichier devraient donc marcher sans          *
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


/*** les 5 fonctionnalit�s suivantes sont plus du sucre syntaxique que du code utile  ***/
/*** sauf � vouloir pouvoir basculer � moindre frais sur une impl�menation des listes ***/
/**** diff�rentes des listes chain�es propos�es dans le cadre de ce projet            ***/

// Liste Vide() { return NULL ; }
// void initVide(Liste *L) { *L = NULL ; }
// bool estVide(Liste l) { return l == NULL ; }
// int premier(Liste l) { return l->valeur ; }
// Liste suite(Liste l) { return l->suite ; }

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

bool UnPlusDeuxEgalTrois (Liste L)
   { return true ; }
   
/********************************************/
/*                                          */
/*             PlusCourte                   */
/*                                          */
/********************************************/

bool PlusCourteRec (Liste L1, Liste L2)
   { return true ; }

/*******/
  
bool PlusCourteIter (Liste L1, Liste L2)
   { return true ; }
   
  
/********************************************/
/*                                          */
/*              Verifiek0                   */
/*                                          */
/********************************************/

bool VerifiekORec (Liste L, int k)
   { return true ; }
   
/*******/

bool VerifiekOIter (Liste L, int k)
   { return true ; }
   

/********************************************/
/*                                          */
/*     NombreTermesAvantZero                */
/*                                          */
/********************************************/

int NTAZ_It (Liste L)
   { return 0 ; }

/*******/

int NTAZ_Rec (Liste L)
   { return 0 ; }

/*******/

int NTAZ_RTSF (Liste L)
   { return 0 ; }

/*******/

int NTAZ_RTSP (Liste L)
   { return 0 ; }


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


// int main(int argc, char** argv)

int main()
{
    Liste l ;

        l = NULL ;
        VireDernier_rec  (&l) ;
        VireDernier_iter (&l) ;
        affiche_rec(l) ; 
        affiche_iter(l) ; 
        printf(" %d \n", longueur_iter(l)) ; 
        printf(" %d \n", longueur_rec(l)) ; 
        VideListe(&l);
        return 0;
}


