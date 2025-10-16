/****************************************************************************************
 *                         RENDU DU TP ALGO : Partie 1                                  *   
 *  Noms du binôme: Thanh Phat DOAN (thanh-phat.doan@universite-paris-saclay.fr)        *
 *                  Naël EL YAZGHI  (nael.el-yazghi@universite-paris-saclay.fr)                                                  *
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
#include <float.h>

/*  #include <stdbool.h>         */
/*  type booléen comme en Algo.c */
typedef enum {false, true} bool;

#include <string.h>
#include <math.h>
#include <time.h>

/*1 - QUELQUE CALCULS SIMPLES*/

/*************************************************************/
/*                                                           */
/*                        Factorielle                        */
/*                                                           */
/*************************************************************/

/*
 * En fait, on n'a pas vraiment besoin de 
 * ces fonctions factorelles, mais on les inclut au cas où.
 **/

long fact1 (int n) { 
    if (n==0) return 1 ;
    else return fact1(n-1) * n ; 
}

// itou avec un argument out => passage par adresse
// // le calcul se fait comme dans la version récursive
// (...(((1*1)*2)*3) ... *n)
void bisfact2(int n, long * r) { 
    if (n==0) *r = 1 ;
    else { 
        bisfact2(n-1,r) ;
        *r = n**r ;  // ou *r*n
    }               // Notez le double sens de * ...
}

long fact2 (int n) { 
    long r ;
    bisfact2(n,&r) ;
    return r ;
}

// variante qui consiste en fait à initialier r beaucoup plus tôt
// du coup, r est inout

void bisfact3(int n, long * r) { 
    if (n==0) {}
    else { 
        bisfact3(n-1,r) ;
         *r = n**r ;  
    }  
}

long fact3 (int n){ 
    long r = 1 ;
    bisfact3(n,&r) ;
    return r ;
}

/*************************************************/

// Ici r est vraiment inout, récursif terminal
// le calcul est différent : (((...((1*n)*(n-1)) ... *3)*2)*1)

void bisfact4(int n, long * r) { 
    if (n==0) {}
    else { 
        *r = n**r ;  
        bisfact4(n-1,r) ; 
    }  
}

long fact4 (int n) { 
    long r = 1 ;
    bisfact4(n,&r) ;
    return r ;
}

/*************************************************/

// et ce code-ci est *** FAUX **** !!! Comprenez-vous pourquoi ?
/* Réponse : À n = 0 (le cas de base), on attribue le résultat r = 1. 
 * Cela rend inutiles les calculs des itérations précédentes.
 * */
void bisfact5(int n, long * r) { 
    if (n==0) *r = 1 ;
    else { 
        *r = n**r ;  
        bisfact5(n-1,r) ; 
    }  
}

long fact5 (int n)
{ long r ;
  bisfact5(n,&r) ;
  return r ;
}

/*************************************************/

// itou en stoquant tout dans un tableau... (très bof en complexité espace)
// c'est juste pour manipuler un peu les tableaux

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

#define VersionsFact 6 // nombre de versions de factorielle

/*************************************************/

long fact(int n , int v )   // numéro de version
{ switch (v)  
   {
   case 1 : return fact1(n) ; break ;
   case 2 : return fact2(n) ; break ; 
   case 3 : return fact3(n) ; break ;
   case 4 : return fact4(n) ; break ;
   case 5 : return fact5(n) ; break ;
   case 6 : return fact6(n) ; break ;
     default : return 0 ; 
   }
}

/***************************************************************/
/*                                                             */
/*                      1.1 Calcul de e                        */
/*                                                             */
/***************************************************************/

/* Note:
 * Pour le calcul de e, on n'a pas nécessairement besoin d'appeler 
 * fact(i) à chaque itération, il suffit de maintenir un 
 * accumulateur factoriel pour obtenir une meilleure complexité O(N) par 
 * rapport à O(N^2) car appeler fact(i) nécessite O(n) chaque i dans [0,n(.
 * L'approximation de e ne nécessite que quelques itérations 
 * mais c'est une bonne pratique quand même.
 * */

 /* "Vous ne sommerez pas jusqu’`a l’infini... Quand et comment vous arrêterez-vous"
        --> On a pris la décision de s'arrêter lorsque 1/n! deviens inférieur à la 
            limite des constantes à virgule flottante, i.e. FLT_MIN = 1,175494351*10^-38 
            pour les flottants. On obtient ce résultat directement en vérifiant la condition
            1/n! = 0 à chaque itération.
    
 ?*/

float Efloat(void) {
    float res = 1;  
    float fact = 1;
    int i = 0;
    while (res + (float)1/fact != res) { 
        res += 1/fact;
        i++;
        fact *= (float)i;
    }
    printf("Efloat. Nb d'approx: %d \n", i - 1);
    return res;
}

/***************************************************************/

double Edouble(void) {
    double res = 1;  
    double fact = 1;
    int i = 0;
    while (res + (double)1/fact != res) { 
        res += 1/fact;
        i++;
        fact *= (double)i;
    }
    printf("Efloat. Nb d'approx: %d \n", i - 1);
    return res;
}

/***************************************************************/

long double Elongdouble(void) {
    long double res = 1;  
    long double fact = 1;
    int i = 0;
    while (res + (long double)1/fact != res) { 
        res += 1/fact;
        i++;
        fact *= (long double)i;
    }
    printf("Efloat. Nb d'approx: %d \n", i - 1);
    return res;
}
/***************************************************************/

/* helper fonctions pour afficher des nombres */
void afficheFloat(float f) {
    printf("%.20f", f);
}
void afficheDouble(double d) { 
    printf("%.30lF", d);
}
void afficheLongDouble(long double ld) { 
    printf("%.40LF", ld);
}

/***************************************************************/
/*                                                             */
/*            1.2: Calcule la suite Y y0 y1 ... yn             */
/*                                                             */
/***************************************************************/

void afficheYfloat(int n) {
    float y = Efloat() - 1;
    for (int i = 1; i < n; i++) {
        y = (float)i * y - 1;
    }

    printf("Suite Y float: ");
    afficheFloat(y);
    printf("\n");
}

/***************************************************************/

void afficheYdouble(int n) {
    double y = Edouble() - 1;
    for (int i = 1; i < n; i++) {
        y = (double)i * y - 1;
    }
    
    printf("Suite Y double: ");
    afficheDouble(y);
    printf("\n");
}

/***************************************************************/

void afficheYlongdouble(int n) {
    long double y = Elongdouble() - 1;
    for (int i = 1; i < n; i++) {
        y = (long double)i * y - 1;
    }
    
    printf("Suite Y long double: ");
    afficheLongDouble(y);
    printf("\n");
}

/* float suite_z_f(void) {
    float y = e_f() - 1 + FLT_EPSILON;
    for (size_t i = 1; i < 30; i++) {
        y = (float)i * y - 1;
    }
    return y;
} */

 /* On constate que le programme affiche bien un résultat qui converge vers 0, avant de diverger vers l'infini
    positif ou négatif selon les cas.
    Mathématiquement, yn converge vers 0 pour y0 = e-1, diverge vers +inf si y0>e-1 et diverge vers -inf sinon.
    Lorsque yn deviens suffisamment petit, l'erreur due à la précision sur les flottants est telle que yn sors 
    de la zone de convergence, ce qui explique le comportement du code.
    */

/***************************************************************/
/*                                                             */
/*                         1.3: SYRACUSE                       */
/*                                                             */
/***************************************************************/

#define CSyr 2025

/**************** 4 versions de int Syracuse(int n) ***********/
/* iteratif */
int SyracuseI(int n) {        
    int syr = CSyr;
    for (int i = 1; i <= n; i++) {
        if (syr % 2) {
            syr = 3*syr + 1;
        }
        else syr = syr/2;
    }
    return syr;
}

/* terminal recursif avec sous-fonction 
 * L'idée est de maintenir un accumulateur (syr) en argument
 * qui stocke le résultat courant du calcul sur chaque itération
 * Pour le cas de base (n == 0), on renvoie accumulateur (syr)
 * */

int SyracuseSF_aux(int n, int syr) {
    if (n == 0) return syr;
    if (syr % 2) {
        return SyracuseSF_aux(n - 1, 3*syr + 1);
    }
    return SyracuseSF_aux(n - 1, syr/2);
}
int SyracuseSF(int n) {         
    return SyracuseSF_aux(n, CSyr);
}

/***************************************************************/

/* terminal recursif avec sous-procédure 
 * L'idée est la même que ci-dessus. Cependant,
 * car on ne peut pas renvoyer une valeur, on modifie
 * directement la valeur syr en passant syr comme un pointeur
 * vers la valeur actuel
 * */

void SyracuseSP_aux(int n, int* syr) {
    if (n == 0) return;
    if ((*syr) % 2) {
        (*syr) = 3 * (*syr) + 1;
        SyracuseSP_aux(n - 1, syr);
    }
    else {
        (*syr) /= 2;
        SyracuseSP_aux(n - 1, syr);
    }
    return;
}
int SyracuseSP(int n) {
    int syr = CSyr;
    SyracuseSP_aux(n, &syr);
    return syr;
}

/***************************************************************/

/* recursif sans sous-fonctionalité 
 * L'idée est de la fonction recursive classique fibonacci
 * */

int SyracuseR(int n) {
    if (n == 0) return CSyr;
    int syr = SyracuseR(n-1);
    if (syr % 2) {
        return 3*syr + 1;
    }
    return syr/2;
}

/***************************************************************/

#define VersionsSyracuse 4  /* # de versions codees */

int Syracuse(int n, int i) { 
    switch (i)  {
        case 1 : return SyracuseI  (n) ; // break;
        case 2 : return SyracuseSF (n) ; // break;  
        case 3 : return SyracuseSP (n) ; // break;  
        case 4 : return SyracuseR  (n) ; // break;  
        default : return 0 ; 
    }
}

/***************************************************************/
/*                                                             */
/*                      1.4: PERMUTATION                       */
/*                                                             */
/***************************************************************/

/* Note: *******************************************************
 * On écrit const int* P comme argument pour indiquer          *
 * que on garantit que on ne modifie pas P et ce que on renvoie* 
 * (la nouveulle permutation de type *int) est alloué          *
 * et doit donc être libéré finalement                         *
 * *************************************************************/

int Int_Lire (void)  // Lecture simpliste d'un entier positif ou nul
                    // ignore les char avant le premier chiffre
                    // s'arrête en absorbant le char après le dernier chiffre
                    // exemple "truc bidule 345x" donne 345
{ char c ; 
 do c = getchar() ; while ( c<'0' || c>'9') ;
 int n = (int) (c - '0') ;
 do { c = getchar() ;
      if ( c<'0' || c>'9') break ;
      n = n*10 + (int) (c - '0') ;
    } while (true) ;
 return n ;
 }
 
/***************************************************************/

int* P_Lire (int n)
{ printf("Input Permutation 0..%d : ", n-1) ;
int* T = (int*) malloc(n*sizeof(int)) ;
  for (int i=0 ; i<n ; i++) T[i] = Int_Lire() ;
  return T ;
}

/***************************************************************/

int* P_Zero (int n)    // Permutation bidon, qui n'est pas une permutation (sauf n=0 ou 1 ...)
{ int* T = (int*) malloc(n*sizeof(int)) ;
  for (int i=0 ; i<n ; i++) T[i] = 0 ;
  return T ;
}

/***************************************************************/

void P_Affiche (const int* P , int n)
{ printf("[") ;
  for (int i=0 ; i<n ; i++) printf(" %d",P[i]) ;
  printf(" ]\n") ;
} 

/***************************************************************/

int* P_identite(int n) {
    int* P = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        P[i] = i;
    }
    return P;
}

/***************************************************************/

int* P_Inverse(const int* P, int n) {
    int* P_1 = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        P_1[P[i]] = i;
    }
    return P_1;
}

/***************************************************************/

/* Ici, au lieu d’appliquer directement R[i] = P[Q[i]], on passe par
 * une variable temporaire tmp, puis on recopie ensuite de tmp vers R.
 * La raison est qu’on veut avoir R = P o Q de sorte que
 * R puisse aussi être P et/ou Q (R apparaît dans le membre de droite).
 * Par ex : P = P o Q , P = P o R , P = P o P
 * En effet, dans ce cas, on ne peut pas accéder et modifier P en même temps. 
 * 
 * Grâce à cela, il facilite et rend notre code, 
 * surtout dans les fonctions de puissance, plus propre et plus concis. */

void P_Compose(const int* P, const int* Q, int* R, int n) {
    int* tmp = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        tmp[i] = P[Q[i]];
    }
    memcpy(R, tmp, sizeof(int) * n);
    free(tmp);
}

/***************************************************************/

/* C'est trivial de trouver cet algo pour vérification de P:
 * Pour chaque P[i], on check
 * - Si P[i] est dans [0, n-1(
 * - Si il existe j != i tq P[i] == P[j]
 * Toutefois, il a le complexité de O(N^2).
 * On peut utiliser plus de mémoire (tableau seen[] de taille N) 
 * pour effectuer cette vérification en O(N):
 * 
 * Pour tout i dans [0, n-1(, seen[i] signifie si on a déjà vu la valeur i dans P[]
 * Pour chaque i, si on l'a pas vu la valeur P[i] 
 *          -> Ok et donc, mise à jour seen[P[i]] = true
 *          -> Sinon, P n'est pas valide
 * */

bool P_Verifie(const int* P, int n) {
    bool seen[n];
    for (int i = 0; i < n; i++) seen[i] = false;
    for (int i = 0; i < n; i++) {
        if (0 <= P[i] && P[i] < n && !seen[P[i]]) {
            seen[P[i]] = true;
        }
        else return false;
    }
    return true;
}

/***************************************************************/
/*                      4 versions de P_power                  */
/***************************************************************/

/* version recursive naïve 
 * Complexité: O(k)
 * */
int* P_power1(const int* P, int n, int k) {      
    if (k == 0) return P_identite(n);
    int* P_k = P_power1(P, n, k-1);
    P_Compose(P_k, P, P_k, n);
    return P_k;
}

/***************************************************************/

/* version iterative naïve 
 * Complexité: O(k)
 * */ 
int* P_power2(const int* P, int n, int k) {  
    if (k == 0) return P_identite(n); 
    int* P_k = P_identite(n);
    for (int i = 1; i <= k; i++) {
        P_Compose(P_k, P, P_k, n);
    }
    return P_k;
}

/***************************************************************/

/* version recursive rapide 
 * On utilise l'algo de Exponentiation rapide. 
 * La formule de récursion est de:
 *
 *                {     P_identite(n)              si k == 0
 * pow(P, n, k) = {     pow(P * P, n, k/2)         si k pair
 *                {     P * pow(P * P, n, (k-1)/2) si k impair
 * 
 * Complexité: O(log(k))
 * */
int* P_power3(const int* P, int n, int k) {  
    if (k == 0) return P_identite(n);
    int* P_k = P_power3(P, n, k/2);
    P_Compose(P_k, P_k, P_k, n);
    if (k % 2) { 
        P_Compose(P_k, P, P_k, n);
    }
    return P_k;
}

/***************************************************************/

/* version iterative rapide 
 * C'est juste de traduire la formule de récursion au style 'bottom up'
 * 
 * pow(P, n, 0)       = P_identite(n)
 * pow(P, n, 2*k)     = pow(P, n, k) * pow(P, n, k) 
 * pow(P, n, 2*k + 1) = pow(P, n, k) * pow(P, n, k) * P
 * 
 * Complexité: O(log(k))
 * */
int* P_power4(const int* P, int n, int k) {  
    int* P_k = P_identite(n);
    if (k == 0) return P_k;
    int* P_base = malloc(sizeof(int) * n);
    P_base = memcpy(P_base, P, sizeof(int) * n);
    while (k > 0) {
        if (k % 2) P_Compose(P_k, P_base, P_k, n);
        P_Compose(P_base, P_base, P_base, n);
        k /= 2;
    }
    free(P_base);
    return P_k;
}
const int VersionsPuissance = 4 ; // nombre de versions de Puissance, soit 4 selon énoncé

/*************************************************/

int* P_power(const int* P, int n, int k, int v )   // version v de 1 à VersionsPuissance
{ switch (v)  
   {
   case 1 : return P_power1(P,n,k) ; break ;
   case 2 : return P_power2(P,n,k) ; break ;
   case 3 : return P_power3(P,n,k) ; break ;
   case 4 : return P_power4(P,n,k) ; break ;
   default : return NULL ; 
   }
}

/***************************************************************/

/* Pour d'avoir la même probabilité sur toutes les permutations
 * On implémente le mélange de Fisher-Yates
 * Complexité: O(n)
 **/
int* P_random(int n) {
    int* P = P_identite(n);
    for (int i = n - 1; i >= 0; i--) {
        int j = rand() % (i + 1);
        
        int tmp = P[i];
        P[i] = P[j];
        P[j] = tmp;
    }
    return P;
}
/************************* SUITE DE TESTS **********************************/

/* helper macro pour faire des
           printf("\n") ;
 tests 
 * assert de C nous aussi suffit mais il quitte le program tout suite si l'un assert échoue
 * en plus, on veut d'avoir plusiers types d'assert */
#define ASSERT(cond) do {   \
    if (!(cond)) {    \
        printf("\nTEST FAILED: %s:%d: %s\n", __FILE__, __LINE__, #cond);   \
        return;     \
    }   \
} while(0)

#define ASSERT_FLOAT_EQ(a,b,eps) ASSERT(fabsf((a) - (b)) < (eps))
#define ASSERT_DOUBLE_EQ(a,b,eps) ASSERT(fabs((a) - (b)) < (eps))
#define ASSERT_LDOUBLE_EQ(a,b,eps) ASSERT(fabsl((a) - (b)) < (eps))

void TEST_1_1(void) {
    printf("1.1 TEST : ");
    /* Note:
     * La fonction exp() de math.h nous suffit aussi, 
     * mais elle nécessite linking lors de la compilation, selon le compilateur et le système 
     * (par exemple, gcc fonctionne sur mon système, mais clang nécessite une liaison). 
     * Ecrire à la main la constante semble chelou mais il est le plus portable pour tester.
     */
    float e_math_f = 2.71828182845904523536028747135266249775724709369995f;
    double e_math_d = 2.71828182845904523536028747135266249775724709369995;
    long double e_math_ld = 2.71828182845904523536028747135266249775724709369995L;
    
    /* float approximation */
    float ef = Efloat();
    ASSERT_FLOAT_EQ(ef, e_math_f, 1e-6f);

    /* double approximation */
    double ed = Edouble(); 
    ASSERT_DOUBLE_EQ(ed, e_math_d, 1e-12);

    /* long double approximation */
    /*  Note:
     *  En fait, l'epsilon pour tester long double devrait être plus petit (de 1e-18 à 1e-21). 
     *  Cependant, sous Windows, long double utilise 8 octets comme double, 
     *  de sorte que cela échouerait sur le système Windows si on testait avec un epsilon aussi strict. 
     * (contrairement à Linux et MacOS qui encodent long double en 16 octets)
     **/
    long double eld = Elongdouble();
    ASSERT_LDOUBLE_EQ(eld, e_math_ld, 1e-12L);

    printf("ALL PASSED!\n");
    printf("----------------------\n");
}

void TEST_1_3(void) {
    printf("1.3 TEST : ");
    
    int syr[5] = {2025, 1519, 15388, 638, 4};
    int at[5] = {0, 3, 10, 100, 1000};
    
    for (int version = 1; version <= VersionsSyracuse; version++) {
        for (int i = 0; i < 5; i++) {
            ASSERT(Syracuse(at[i], version) == syr[i]);
        }    
    }

    printf("ALL PASSED!\n");
    printf("----------------------\n");
}
bool is_equal(const int* P, const int* Q, int n) {
    for (int i = 0; i < n; i++) {
        if (P[i] != Q[i]) return false;
    }
    return true;
}
void TEST_1_4(void) {
    printf("1.4 TEST : ");

    /* P_identite test */
    int* P = P_identite(7);
    for (int i = 0; i < 7; i++) ASSERT(P[i] == i);
    free(P);

    P = P_identite(10);
    for (int i = 0; i < 10; i++) ASSERT(P[i] == i);
    free(P);

    /* P_inverse test */
    int Q[6] = {4, 5, 2, 1, 3, 0};
    int Q_1_test[6] = {5, 3, 2, 4, 0, 1};
    int* Q_1 = P_Inverse(Q, 6);
    ASSERT(P_Verifie(Q_1, 6));
    ASSERT(is_equal(Q_1, Q_1_test, 6));
    free(Q_1);

    /* P_compose test */
    int X[6] = {0, 5, 3, 4, 2, 1};
    int Y[6] = {4, 5, 2, 1, 3, 0};
    int Z_test[6] = {2, 1, 3, 5, 4, 0};
    int* Z = malloc(sizeof(int) * 6);
    P_Compose(X, Y, Z, 6);
    ASSERT(P_Verifie(Z, 6));
    ASSERT(is_equal(Z, Z_test, 6));
    free(Z);

    /* P_verifie test */
    int M[4] = {1, 0, 3, 1};
    int N[4] = {0, 1, 23, 42};
    ASSERT(!P_Verifie(M, 4));
    ASSERT(!P_Verifie(N, 4));

    /* P_power test */
    int A[8] = {2, 6, 1, 0, 7, 3, 5, 4};
    int A_test[6][8] = {
        {0, 1, 2, 3, 4, 5, 6, 7},
        {2, 6, 1, 0, 7, 3, 5, 4}, 
        {1, 5, 6, 2, 4, 0, 3, 7}, 
        {6, 3, 5, 1, 7, 2, 0, 4}, 
        {5, 0, 3, 6, 4, 1, 2, 7}, 
        {3, 2, 0, 5, 7, 6, 1, 4},
    };

    int* A_check = NULL;
    for (int version = 1; version <= VersionsPuissance; version++) {
        for (int i = 0; i < 6; i++) { 
            A_check = P_power(A, 8, i, version);
            ASSERT(P_Verifie(A_check, 8));
            ASSERT(is_equal(A_check, A_test[i], 8));
            free(A_check);
            }
    }

    /* P_random test */
    int* R = NULL;
    R = P_random(9);
    ASSERT(P_Verifie(R, 9));
    free(R);

    R = P_random(100);
    ASSERT(P_Verifie(R, 100));
    free(R);

    printf("ALL PASSED!\n");
    printf("----------------------\n");
}

void TEST(void) {
    printf("******** Suites de tests automatiques *******\n");
    TEST_1_1();
    TEST_1_3();
    TEST_1_4();
    printf("*********************************************\n\n");
    return;
}

int main(void) {
    srand(time(NULL));      // pour P_random d'etre "plus" randomise
    
    /******** Nos propres suites de tests automatiques (memory-leak free) *********/
    TEST();
    /******************************************************************************/


    /************************ Suites de tests données du TP ************************/
    
    /* Note: ***********************************************************************
     * Les tests donnés par le fichier Algo1.c devraient marcher sans modification *
     * Mettez "if true" vs "if false" selon que vous voulez les tests ou pas       *
     * *****************************************************************************/

     printf("******** Suites de tests données du TP *******\n");

     /*************************  taille des nombres  ********************************/
      
    if (true) {  
           printf("tailles des nombres (peuvent varier selon le compilo) :\n") ;
           
           printf("short : %d octets\n", (int) sizeof(short));
           printf("int : %d octets\n", (int) sizeof(int));
           printf("long : %d octets\n", (int) sizeof(long));
           printf("long long : %d octets\n", (int) sizeof(long long));
           printf("float : %d octets\n", (int) sizeof(float));
           printf("double : %d octets\n", (int) sizeof(double));
           printf("long double : %d octets\n", (int) sizeof(long double));
           printf("\n") ;
     
           printf("limite des flottants (peuvent varier selon le compilo) :\n") ;
                 
           float z1 ;
           double z2 ;
           long double z3 ;
           
           int cpt ;

           z1 = 2.0 ; cpt = 0 ;
           while ( z1 > 1 ) { z1 = (z1-1)/10+1 ; cpt ++ ; }
           printf("1+1/10^c devient 1 a partir de c=%d pour les float\n", cpt) ; 
        
           z2 = 2.0 ; cpt = 0 ;
           while ( z2 > 1 ) { z2 = (z2-1)/10+1 ; cpt ++ ; }
           printf("1+1/10^c devient 1 a partir de c=%d pour les double\n", cpt) ; 
          
           z3 = 2.0 ; cpt = 0 ;
           while ( z3 > 1 ) { z3 = (z3-1)/10+1 ; cpt ++ ; }
           printf("1+1/10^c devient 1 a partir de c=%d pour les long double\n", cpt) ; 
           
           z1 = 1.0 ; cpt = 0 ;
           while ( z1 > 0 ) { z1 = z1/10 ; cpt ++ ; }
           printf("1/10^c devient 0 a partir de c=%d pour les float\n", cpt) ; 
           
           z2 = 1.0 ; cpt = 0 ;
           while ( z2 > 0 ) { z2 = z2/10 ; cpt ++ ; }
           printf("1/10^c devient 0 a partir de c=%d pour les double\n", cpt) ; 
           
           z3 = 1.0 ; cpt = 0 ;
           while ( z3 > 0 ) { z3 = z3/10 ; cpt ++ ; }
           printf("1/10^c devient 0 a partir de c=%d pour les long double\n", cpt) ; 

           printf("\n") ;

    }

    /************************  factorielle  *************************/

    if (true) {

         printf("factorielles de 0, 1, 2, 3, 4, 5, 10, 30, 100, %d codes : \n", VersionsFact) ;
         for (short v=1 ; v<=VersionsFact ; v++ )
            printf("version %d : %ld %ld %ld %ld %ld %ld %ld %ld %ld \n",
                v, fact(0,v), fact(1,v), fact(2,v), fact(3,v), fact(4,v), fact(5,v), fact(10,v), fact(30,v), fact(100,v)) ; 
         printf("\n") ;
    }


    /******************    Autour de e      *******************************/

      // d'après google,
      // e = 2,7182818284 5904523536 0287471352 6624977572 4709369995 
      //       9574966967 6277240766 3035354759 4571382178 5251664274

    if (true) {  

            printf(" e1 = %.20f  \n", Efloat() ) ;
            printf(" e3 = %.30lf \n", Edouble() ) ; 
            printf(" e3 = %.40LF \n", Elongdouble() ) ; 
            printf("\n") ;

            
    }

    if (true) {  
                printf("Valeurs de Y, selon float, double, longdouble :\n") ;
                afficheYfloat(30) ;
                afficheYdouble(30) ;
                afficheYlongdouble(30) ;
                printf("\n") ;
    }
     

    /******************************* Permutations **************************/
    
    /* Note: ***************************************************************
     * Les tests P_power ici sont mal conçus. En effet, comme P_power      *
     * alloue toujours de la nouvelle mémoire pour la permutation P^k mais *
     * on ne la capture pas, on ne peut pas la libérer à la fin.           *
     ***********************************************************************/
    if (false) 

    { printf("dim des permutations ? : \n") ; 
       int dim = Int_Lire() ;
       
       printf("Identite : ") ; 
       P_Affiche(P_identite(dim),dim) ;
        
       int * tmp1 = P_Lire(dim) ; 
       P_Affiche(tmp1,dim) ;    
       printf("Permutation correcte ? : %d \n", P_Verifie(tmp1,dim)) ;
       
       int * tmp2 = P_Lire(dim) ;
       int * tmp3 = (int *) malloc(dim*sizeof(int)) ;
       P_Compose(tmp1,tmp2,tmp3,dim) ;
       printf("Composee des permutations en input :" ) ; P_Affiche(tmp3,dim) ; 
       
       int * tmp = P_random(dim) ; 
             printf("permutation aleatoire :" ) ; P_Affiche(tmp,dim) ;

         printf ("puissances 0, 1, 2, 3, dim, dim! \n") ;       
         for(int v=1 ; v<= VersionsPuissance ; v++)
        { printf ("version puissance %d \n", v) ;
          P_Affiche(P_power(tmp,dim,0,v),dim) ;    // note : fuite memoire ici....
          P_Affiche(P_power(tmp,dim,1,v),dim) ;  
          P_Affiche(P_power(tmp,dim,2,v),dim) ;  
          P_Affiche(P_power(tmp,dim,3,v),dim) ;  
          P_Affiche(P_power(tmp,dim,dim,v),dim) ;  
          P_Affiche(P_power(tmp,dim,fact1(dim),v),dim) ;   
         }

       free(tmp1) ; free(tmp2) ; free(tmp3) ; free(tmp) ; 
       
           printf("\n") ;


    }

    /******************    Syracuse    *******************************/
     
     
             
    if (false) {   // Test de toutes les versions
     
        for(int v=1 ; v<=VersionsSyracuse ; v++)  // numéro de version       
            {        
            printf("\n Syr %d \n", v) ;        
            for(int j=0 ; j<=3 ; j++) 
            printf("Syr%d(%d) = %d \n", v, j, Syracuse(j,v)) ;  
            
            long n = 1 ; 
            for (int j=1 ; j<=3 ; j++)
               { n = n*10 ;
                 printf("Syr%d(10^%d) = %d \n", v, j, Syracuse(n,v)) ; }  
                 
            printf("\n") ;
     
                 
                      
           }
    }

    printf("*********************************************\n\n");
    /***********************************************************************/

    return 0;
}
