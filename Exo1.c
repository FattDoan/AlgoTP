/****************************************************************************************
 *                         RENDU DU TP ALGO : Partie 1                                  *   
 *  Noms du binôme: Thanh Phat DOAN (thanh-phat.doan@universite-paris-saclay.fr)        *
 *                  Naël EL YAZGHI  ()                                                  *
 *                                                                                      *
 *  Le code est conforme à la norme C99 et a été compilé avec les options               *
 *  les plus strictes: -std=c99 -pedantic -Wall -Wextra -Werror.                        *
 *  Les tableaux à taille variable (VLA) n’ont pas été utilisés.                        *
 *  Les fuites mémoire ont été vérifiées avec Valgrind (--leak-check=full)              *
 *                                                                                      *
*****************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>

void print_f(float f) {
    printf("float:       %.9g  (%.9f)\n", f, f);
}
void print_d(double d) { 
    printf("double:      %.17g  (%.17f)\n", d, d);
}
void print_ld(long double ld) { 
    printf("long double: %.21Lg  (%.21Lf)\n", ld, ld);
}
/***************************************************************/
/* 1.1: Calcule e */
float e_f(void) {
    float res = 1;  
    float fact = 1;
    for (int i = 1; i < 10; i++) {
        fact *= (float)i;
        res += 1/fact;
    }
    return res;
}
double e_d(void) {
    double res = 1;  
    double fact = 1;
    for (int i = 1; i < 17; i++) {
        fact *= (double)i;
        res += 1/fact;
    }
    return res;
}
long double e_ld(void) {
    long double res = 1;  
    long double fact = 1;         
    for (int i = 1; i < 30; i++) {
        fact *= (long double)i;
        res += 1/fact;
    }
    return res;
}
/***************************************************************/
/* 1.2: Calcule la suite y0 y1 ... yn */
float suite_y_f(void) {
    float y = e_f() - 1;
    for (size_t i = 1; i < 30; i++) {
        /*print_f(y);*/
        y = (float)i * y - 1;
    }
    return y;
}
double suite_y_d(void) {
    double y = e_d() - 1;
    for (size_t i = 1; i < 30; i++) {
        /*print_d(y);*/
        y = (double)i * y - 1;
    }
    return y;
}
long double suite_y_ld(void) {
    long double y = e_ld() - 1;
    for (size_t i = 1; i < 30; i++) {
        /*print_ld(y);*/
        y = (long double)i * y - 1;
    }
    return y;
}
float suite_z_f(void) {
    float y = e_f() - 1 + FLT_EPSILON;
    for (size_t i = 1; i < 30; i++) {
        /*print_f(y);*/
        y = (float)i * y - 1;
    }
    return y;
}
/***************************************************************/
/* 1.3 : Fonction Syracuse */
#define CSyr 2025

/**************** 4 versions de int Syracuse(int n) ***********/
/* iteratif */
int Syracuse_iter(int n) {        
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
 * qui stocke le résultat du calcul sur chaque itération
 * Pour le cas de base (n == 0), on renvoie accumulateur (syr)
 * */
int Syracuse_rec_term_f_aux(int n, int syr) {
    if (n == 0) return syr;
    if (syr % 2) {
        return Syracuse_rec_term_f_aux(n - 1, 3*syr + 1);
    }
    return Syracuse_rec_term_f_aux(n - 1, syr/2);
}
int Syracuse_rec_term_f(int n) {         
    return Syracuse_rec_term_f_aux(n, CSyr);
}

/* terminal recursif avec sous-procédure 
 * L'idée est la même que ci-dessus. Cependant,
 * car on ne peut pas renvoyer une valeur, on modifie
 * directement la valeur syr en passant syr comme un pointeur
 * */
void Syracuse_rec_term_p_aux(int n, int* syr) {
    if (n == 0) return;
    if ((*syr) % 2) {
        (*syr) = 3 * (*syr) + 1;
        Syracuse_rec_term_p_aux(n - 1, syr);
    }
    else {
        (*syr) /= 2;
        Syracuse_rec_term_p_aux(n - 1, syr);
    }
    return;
}
int Syracuse_rec_term_p(int n) {
    int syr = CSyr;
    Syracuse_rec_term_p_aux(n, &syr);
    return syr;
}

/* recursif sans sous-fonctionalité 
 * L'idée est de la fonction recursive classique fibonacci
 * */
int Syracuse_rec(int n) {
    if (n == 0) return CSyr;
    int syr = Syracuse_rec(n-1);
    if (syr % 2) {
        return 3*syr + 1;
    }
    return syr/2;
}
/***************************************************************/
/* 1.4 : Permutation */
int* P_identite(int n) {
    int* P = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        P[i] = i;
    }
    return P;
}
int* P_inverse(int* P, int n) {
    int* P_1 = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        P_1[P[i]] = i;
    }
    return P_1;
}
void P_compose(const int* P, const int* Q, int* R, int n) {
    int* tmp = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        tmp[i] = P[Q[i]];
    }
    memcpy(R, tmp, sizeof(int) * n);
    free(tmp);
}

/* C'est trivial de trouver cet algo:
 * Pour chaque P[i], on check
 * - Si P[i] est dans [0, n-1(
 * - Si il existe j != i tq P[i] == P[j]
 * Toutefois, il a le complexité de O(N^2).
 * On peut utiliser plus de mémoire (tableau seen[]) pour effectuer cette vérification en O(N):
 * Pour tout i dans [0, n-1(, seen[i] signifie si on a déjà vu la valeur i dans P[]
 * Pour chaque P[i], si on l'a pas vu -> Ok et donc, mise à jour seen[P[i]] = true
 *                                    -> Sinon, P n'est pas valide
 * */
bool P_verifie(int* P, int n) {
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

/********* 4 versions de P_power **************/

/* version recursive naïve 
 * Complexité: O(k)
 * */
int* P_power_rec(int* P, int n, int k) {      
    if (k == 0) return P_identite(n);
    int* P_k = P_power_rec(P, n, k-1);
    P_compose(P_k, P, P_k, n);
    return P_k;
}

/* version iterative naïve 
 * Complexité: O(k)
 * */ 
int* P_power_iter(int* P, int n, int k) {  
    if (k == 0) return P_identite(n); 
    int* P_k = P_identite(n);
    for (int i = 1; i <= k; i++) {
        P_compose(P_k, P, P_k, n);
    }
    return P_k;
}
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
int* P_power_rec_rapide(int* P, int n, int k) {  
    if (k == 0) return P_identite(n);
    int* P_k = P_power_rec_rapide(P, n, k/2);
    P_compose(P_k, P_k, P_k, n);
    if (k % 2) { 
        P_compose(P_k, P, P_k, n);
    }
    return P_k;
}

/* version iterative rapide 
 * C'est juste de traduire la formule de récursion au style 'bottom up'
 * 
 * pow(P, n, 0)       = P_identite(n)
 * pow(P, n, 2*k)     = pow(P, n, k) * pow(P, n, k) 
 * pow(P, n, 2*k + 1) = pow(P, n, k) * pow(P, n, k) * P
 * 
 * Complexité: O(log(k))
 * */
int* P_power_iter_rapide(const int* P, int n, int k) {  
    int* P_k = P_identite(n);
    if (k == 0) return P_k;
    int* P_base = malloc(sizeof(int) * n);
    P_base = memcpy(P_base, P, sizeof(int) * n);
    while (k > 0) {
        if (k % 2) P_compose(P_k, P_base, P_k, n);
        P_compose(P_base, P_base, P_base, n);
        k /= 2;
    }
    free(P_base);
    return P_k;
}
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

/* helper macro pour faire des tests 
 * assert de C nous aussi suffit mais il quitte le program tout suite 
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
     * Ceci est la méthode la plus portable.
     */
    float e_math_f = 2.71828182845904523536f;
    double e_math_d = 2.71828182845904523536;
    long double e_math_ld = 2.71828182845904523536L;
    
    /* float approximation */
    float ef = e_f();
    ASSERT_FLOAT_EQ(ef, e_math_f, 1e-6f);

    /* double approximation */
    double ed = e_d(); 
    ASSERT_DOUBLE_EQ(ed, e_math_d, 1e-12);

    /* long double approximation */
    /*  Note:
     *  En fait, l'epsilon pour tester long double devrait être plus petit (de 1e-18 à 1e-21). 
     *  Cependant, sous Windows, long double utilise 8 octets comme double, 
     *  de sorte que cela échouerait sur le système Windows si on testait avec un epsilon aussi strict. 
     * (contrairement à Linux et MacOS qui encodent long double en 16 octets)
     **/
    long double eld = e_ld();
    ASSERT_LDOUBLE_EQ(eld, e_math_ld, 1e-12L);

    printf("ALL PASSED!\n");
    printf("----------------------\n");
}

void TEST_1_3(void) {
    printf("1.3 TEST : ");
    
    int syr[5] = {2025, 1519, 15388, 638, 4};
    int at[5] = {0, 3, 10, 100, 1000};

    for (int i = 0; i < 5; i++) {
        ASSERT(Syracuse_iter(at[i]) == syr[i]);
        ASSERT(Syracuse_rec_term_f(at[i]) == syr[i]);
        ASSERT(Syracuse_rec_term_p(at[i]) == syr[i]);
        ASSERT(Syracuse_rec(at[i]) == syr[i]);
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
    int* Q_1 = P_inverse(Q, 6);
    ASSERT(P_verifie(Q_1, 6));
    ASSERT(is_equal(Q_1, Q_1_test, 6));
    free(Q_1);

    /* P_compose test */
    int X[6] = {0, 5, 3, 4, 2, 1};
    int Y[6] = {4, 5, 2, 1, 3, 0};
    int Z_test[6] = {2, 1, 3, 5, 4, 0};
    int* Z = malloc(sizeof(int) * 6);
    P_compose(X, Y, Z, 6);
    ASSERT(P_verifie(Z, 6));
    ASSERT(is_equal(Z, Z_test, 6));
    free(Z);

    /* P_verifie test */
    int M[4] = {1, 0, 3, 1};
    int N[4] = {0, 1, 23, 42};
    ASSERT(!P_verifie(M, 4));
    ASSERT(!P_verifie(N, 4));

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
    for (int i = 0; i < 6; i++) { 
        A_check = P_power_iter(A, 8, i);
        ASSERT(P_verifie(A_check, 8));
        ASSERT(is_equal(A_check, A_test[i], 8));
        free(A_check);

        A_check = P_power_rec(A, 8, i);
        ASSERT(P_verifie(A_check, 8));
        ASSERT(is_equal(A_check, A_test[i], 8));
        free(A_check);        
        
        A_check = P_power_iter_rapide(A, 8, i);
        ASSERT(P_verifie(A_check, 8));
        ASSERT(is_equal(A_check, A_test[i], 8));
        free(A_check);
        
        A_check = P_power_rec_rapide(A, 8, i);
        ASSERT(P_verifie(A_check, 8));
        ASSERT(is_equal(A_check, A_test[i], 8));
        free(A_check);
    }     

    /* P_random test */
    int* R = NULL;
    R = P_random(9);
    ASSERT(P_verifie(R, 9));
    free(R);

    R = P_random(100);
    ASSERT(P_verifie(R, 100));
    free(R);

    printf("ALL PASSED!\n");
    printf("----------------------\n");
}

void TEST(void) {
    TEST_1_1();
    TEST_1_3();
    TEST_1_4();
}

int main(void) {
    srand(time(NULL));

    TEST();
    return 0;
}
