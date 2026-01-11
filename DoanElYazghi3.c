/**************************************************************************
 *                     RENDU DU TP ALGO : Partie 3                        *   
 *  Noms du binôme:                                                       *
 *  - Thanh Phat DOAN (thanh-phat.doan@universite-paris-saclay.fr)        *
 *  - Naël EL YAZGHI  (nael.el-yazghi@universite-paris-saclay.fr)         *
 *                                                                        *               
 *  Le code est conforme à la norme C99 et a été compilé avec les options *               
 *  les plus strictes: -std=c99 -pedantic -Wall -Wextra -Werror.          *
 *  Les fuites mémoire ont été vérifiées avec Valgrind (--leak-check=full)*
 *                                                                        *
***************************************************************************/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/********************************************************/
/*                                                      */
/*                    Type booléen                      */
/*                                                      */
/********************************************************/

typedef enum {false, true} bool;    
// NOTE: gcc peut se plaindre si on inclut pas -std=c99

/*******************************************************/
/*                                                     */
/*            definition type bloc_image               */
/*                                                     */
/*******************************************************/

typedef struct bloc_image { 
    bool blanc;
    struct bloc_image* Im[4];
} bloc_image;

typedef bloc_image* image;

/********************************************************/
/*                                                      */
/*           Les fonctions utilitaires                  */ 
/*           (non requis par les consignes)             */
/*                                                      */
/********************************************************/
int max(int a, int b) {
    return (a > b) ? a : b;
}
/********************************************************/
// Liberer la mémoire
void FreeImg(image img) {
    if (img != NULL) {
        for (int i = 0; i < 4; i++) {
            FreeImg(img->Im[i]);
        }
        free(img);
    }
    return;
}
/********************************************************/
int Profondeur(image img) {
    if (img == NULL || img->blanc) return 0;
    int max_prof = max(max(Profondeur(img->Im[0]), Profondeur(img->Im[1])),
                       max(Profondeur(img->Im[2]), Profondeur(img->Im[3])));
    return 1 + max_prof;
}
/********************************************************/
// Dessine() : dessine l'image dans le terminal
// avec l'aide de la fonction auxiliaire DessineLigne()
// qui dessine une ligne y de l'image img de profondeur p et de taille n*n
// '■' pour noir, '□' pour blanc
// Cette fonction est principalement utile pour le débogage
void DessineLigne(image img, int y, int p, int n) {
    if (img == NULL) {
        for (int i = 0; i < n; i++) printf("■  ");
        return;
    }
    if (img->blanc) {
        for (int i = 0; i < n; i++) printf("□  ");
        return;
    }

    // Si l'image n'est ni noire ni blanche, 
    // on divise en 4 quadrants
    // mais on ne dessine que 
    // les 2 quadrants correspondants à la ligne y
   // (car on dessine ligne par ligne)

    //  L'image img de profondeur p et de taille n*n est divisée en 4 quadrants:
    //  |----------------------------------|---------------------------------|
    //  |  DessineLigne(img, y, p-1, n/2)  | DessineLigne(img, y, p-1, n/2)  |  
    //  |----------------------------------|---------------------------------|
    //  |  DessineLigne(img, y', p-1, n/2) | DessineLigne(img, y', p-1, n/2) | 
    //  |----------------------------------|---------------------------------|
    // 
    //  où n = y + y'

    int n2 = n / 2;
    if (y < n2) {  // top 2 quadrants
        DessineLigne(img->Im[0], y, p - 1, n2);
        DessineLigne(img->Im[1], y, p - 1, n2);
    } else {  // bottom 2 quadrants
        DessineLigne(img->Im[2], y - n2, p - 1, n2);
        DessineLigne(img->Im[3], y - n2, p - 1, n2);
    }
}

void Dessine(image img) {
    int p = Profondeur(img);
    int n = 1 << p;    // l'image est toujours de taille n*n (n = 2^p)
    for (int i = 0; i < n; i++) {
        DessineLigne(img, i, p, n);
        printf("\n");
    }
    printf("\n");
}
/********************************************************/
// Vérifie si deux images sont égales
// <!> Cette fonction teste l'égalité structurelle stricte des images
// et non pas l'égalité visuelle.
// (2 images peuvent être visuellement identiques
// mais avoir des structures différentes dans le quadtree)
bool equals(image img1, image img2) {
    if (img1 == NULL && img2 == NULL) return true;
    if (img1 == NULL || img2 == NULL) return false;
    if (img1->blanc != img2->blanc) return false;

    for (int i = 0; i < 4; i++) {
        if (!equals(img1->Im[i], img2->Im[i])) {
            return false;
        }
    }
    return true;
}

/********************************************************/
/*                                                      */
/*                      Wht()                           */
/*                                                      */
/********************************************************/
image Wht() {
    image img = (image)malloc(sizeof(bloc_image));
    if (img == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }
    img->blanc = true;
    for (int i = 0; i < 4; i++) img->Im[i] = NULL;
    return img;
}
/********************************************************/
/*                                                      */
/*                      Blk()                           */
/*                                                      */
/********************************************************/
image Blk() {
    return NULL;
}
/********************************************************/
/*                                                      */
/*                  Cut(i0,i1,i2,i3)                    */
/*                                                      */
/********************************************************/
image Cut(image i0, image i1, image i2, image i3) {
    image img = (image)malloc(sizeof(bloc_image));
    if (img == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    img->blanc = false;
    img->Im[0] = i0; img->Im[1] = i1;
    img->Im[2] = i2; img->Im[3] = i3;
    return img;
}

/* NOTE:
 * D'après le feedback reçu du pré-rendu, 
 * les fonctions Wht, Blk et Cut constitueront nos briques de base 
 * pour toutes les fonctions ci-dessous 
 * et aucun malloc supplémentaire ne sera nécessaire. 
 * ****************************************************************/


/********************************************************/
/*                                                      */
/*                      Affiche(image)                  */
/*                                                      */
/********************************************************/
void Affiche(image img) {
    if (img == NULL) {
        printf("Z");
        return;
    }
    if (img->blanc == true) {
        printf("o");
        return;
    }
    printf("*");
    for (int i = 0; i < 4; i++) {
        Affiche(img->Im[i]);
    }
}
/********************************************************/
/*                                                      */
/*                   ProfAffiche()                      */
/*                                                      */
/********************************************************/
void ProfAffiche_aux(image img, int depth) {
    if (img == NULL) {
        printf("Z%d ", depth);
        return;
    }
    if (img->blanc == true) {
        printf("o%d ", depth);
        return;
    }
    printf("*%d ",depth);
    for (int i = 0; i < 4; i++) {
        ProfAffiche_aux(img->Im[i], depth+1);
    }
}
void ProfAffiche(image img) {
    ProfAffiche_aux(img, 0);
}

/********************************************************/
/*                                                      */
/*                      Lecture()                       */
/*                                                      */
/********************************************************/
// Pour Lecture(), on définit une fonction auxiliaire Lecture_aux
// pour rendre une image à partir d'une chaîne de caractères

/* NOTE: 
 * D'après le commentaire reçu du pré-rendu,                                 
 * on est d'accord que notre fonction de Lecture est                         
 * plus compliquée que nécessaire,                                           
 * car on peut simplement lire l'entrée directement sans passer              
 * par l'étape intermédiaire consistant à stocker l'entrée dans une chaîne.  
 *                                                                           
 * Cependant, cette approche nous permet d'initialiser une image            
 * directement par une chaîne (la fonction Image) dans le code sans avoir 
 * à saisir l'entrée à chaque fois,
 * ce qui est très pratique pour les tests et le débogage. 
 *****************************************************************************/

image Lecture_aux(char** arr) {
    char c = **arr;
    // ignore les caractères non pertinents 
    while (c != '\0' && c != 'o' && c != 'Z' && c != '*') {
        (*arr)++;
        c = **arr;
    }
    if (c == '\0') {
        return NULL; // fin de la chaîne
    }

    (*arr)++;
    if (c == 'Z') {
        return Blk();
    }
    if (c == 'o') {
        return Wht();
    }
    if (c == '*') {
        image i0 = Lecture_aux(arr);
        image i1 = Lecture_aux(arr);
        image i2 = Lecture_aux(arr);
        image i3 = Lecture_aux(arr);
        return Cut(i0, i1, i2, i3);
    }
    return NULL;
}

// car on ne peut pas connaitre la taille de l'entrée à l'avance,
// on lit d'abord toute l'entrée dans un tableau de taille dynamique
// qui peut s'agrandir si besoin (si n (taille actuelle) atteint la capacité)
image Lecture() {
    int c, capacity = 16, n  = 0;
    char* arr = malloc(capacity * sizeof(char));

    while ((c = getchar()) != '\n' && c != EOF) {
        if (n + 1 >= capacity) { 
            capacity *= 2;
            char *new_arr = realloc(arr, capacity);
            arr = new_arr;
        }
        arr[n++] = c;
    }
    arr[n] = '\0'; // terminer la chaîne
    char *ptr = arr;

    image res = Lecture_aux(&ptr);
    free(arr);
    return res;
}

image Image(char* str) {
    return Lecture_aux(&str);
}

/********************************************************/
/*                                                      */
/*           DessinNoir() et DessinBlanc()              */
/*                                                      */
/********************************************************/

// La plupart des fonctions quadtree auront le même structure que celles-ci,
// avec des cas de base et des appels récursifs sur 4 sous-images de l'image.
bool DessinNoir(image img) {
    if (img == NULL) {
        return true;
    }
    if (img->blanc == true) {
        return false;
    }

    return DessinNoir(img->Im[0]) && DessinNoir(img->Im[1]) 
        && DessinNoir(img->Im[2]) && DessinNoir(img->Im[3]);
}
bool DessinBlanc(image img) {
    if (img == NULL) {
        return false;
    }
    if (img->blanc == true) {
        return true;
    }
    return DessinBlanc(img->Im[0]) && DessinBlanc(img->Im[1]) 
        && DessinBlanc(img->Im[2]) && DessinBlanc(img->Im[3]);
}
/********************************************************/
/*                                                      */
/*                    QuotaNoir()                       */
/*                                                      */
/********************************************************/
// l'idee meme que de DessinNoir
float QuotaNoir(image img) {
    if (img == NULL) {
        return 1.f;
    }
    if (img->blanc == true) {
        return 0.f;
    }

    float res = 0.f;
    for (int i = 0; i < 4; i++) {
        res += QuotaNoir(img->Im[i]) / 4;
    }
    return res;

}
/********************************************************/
/*                                                      */
/*                      Copie()                         */
/*                                                      */
/********************************************************/
// Renvoie un Cut avec des copies des sous-images
image Copie(image img) {
    if (img == NULL) return Blk();
    if (img->blanc) return Wht();

    return Cut(Copie(img->Im[0]), Copie(img->Im[1]),
               Copie(img->Im[2]), Copie(img->Im[3]));
}
/********************************************************/
/*                                                      */
/*                      Diagonale()                     */
/*                                                      */
/********************************************************/
// Si on note D(p) l'image diagonale de profondeur p,
// Alors D(p) est construite récursivement comme suit:
// D(p) = Cut(
//            D(p-1), Wht(), 
//            Wht(), D(p-1)
//           )
// D(1) = ZooZ
image Diagonale(int p) {
    if (p == 1) return Cut(Blk(), Wht(), Wht(), Blk());

    image tmp = Diagonale(p-1);
    return Cut(tmp, Wht(), Wht(), Copie(tmp));
}

/********************************************************/
/*                                                      */
/*                  SimplifieProfP()                    */
/*                                                      */
/********************************************************/

/* img est un argument de type inout 
 * L'idee est suivante:
 * SimplifieProfP(img, p) =
 *  if p == 0:
 *      if img est monochrome noir: img = Blk() (*)
 *      else if img est monochrome blanc: img = Wht()
 *      else: ne fait rien
 *
 *  else:
 *      if img est monochrome et n'est PAS COMPOSÉ (**), ne fait rien
 *      else divise img en 4 sous-images SousImg[0..3]
 *          et rappelle récursivement:
 *          SimplifieProfP(SousImg[i], p-1) pour i = 0,1,2,3
 *
 *  (*): On libère la mémoire si on a besoin de remplacer par une nouvelle img
 *  
 *  (**): Nous ne nous soucions pas du fait qu'il soit monochrome MAIS composé 
 *  (ce qui devrait être vérifié par DessinNoir et DessinBlanc) 
 *  car p n'est pas égal à 0. Il faut noter que p n'est pas la profondeur 
 *  de la sous-image actuelle, mais le nombre de niveaux qu'il nous reste 
 *  jusqu'à ce que p = 0, moment où nous pouvons la simplifier.
 *****************************************************************************/
void SimplifieProfP(image* img, int p) {
    if (*img == NULL || (*img)->blanc == true) {
        return;
    }
    if (p == 0) {
        if (DessinNoir(*img)) {
            FreeImg(*img);
            *img = Blk();
        } else if (DessinBlanc(*img)) {
            FreeImg(*img);
            *img = Wht();
        }
        // sinon ne fait rien car pas monochrome
        return;
    }
    
    for (int i = 0; i < 4; i++) {
        SimplifieProfP(&((*img)->Im[i]), p - 1);
    }
}

/********************************************************/
/*                                                      */
/*                  Incluse()                           */
/*                                                      */
/********************************************************/
// On assume pour Incluse(img1, img2) que img1 et img2 ont la même taille
/* Pour le cas général, on effectue des appels récursifs sur les sous-images
 * correspondantes de img1 et img2 afin de les comparer.
 *
 * Les cas de base pour vérifier que 
 * img1 est incluse dans img2 sont les suivants:
 *
 * 1. img1 est noir => (img1 est incluse <=> img2 est noir)
 * 2. img1 est blanc ou img2 est noir => True 
 * 3. img2 est blanc => (img1 est incluse <=> img1 est blanc)
 *
 * Pour tester si une image est noire ou blanche, on pourrait utiliser les
 * fonctions DessinNoir et DessinBlanc définies précédemment. Cependant,
 * ces fonctions parcourent récursivement l’arbre et ont une complexité
 * en O(log n) pour une image de taille n.
 *
 * Si on utilisait DessinNoir/Blanc à chaque appel récursif, 
 * la complexité totale de Incluse serait alors en O((log n)^2).
 *
 * On peut l'améliorer un peu en se contentant de vérifier si
 * l'image est noire (img == NULL), blanche (img->blanc == true)
 * (cela implique l'image n'est PAS COMPOSÉE).
 * Les appels à DessinNoir/Blanc ne sont effectués que lorsque ces tests 
 * permettent de conclure immédiatement, c-a-d uniquement dans les cas 
 * terminaux où aucun appel récursif supplémentaire n’est nécessaire.
 * 
 * En particulier, si l’une des images est composée 
 * (qui peut toujours être monochrome), la descente récursive
 * est poursuivie et le résultat n’est déterminé qu’à partir des sous-images.
 * 
 * Cette petite optimisation évite des parcours inutiles de l’arbre et 
 * permet de ramener la complexité globale à O(log n).
 *
 ***************************************************************************/
bool Incluse(image img1, image img2) {
    if (img1 == NULL) {
        return DessinNoir(img2); 
    }
    if (img1->blanc == true || img2 == NULL) {
        return true; 
    }

    if (img2->blanc == true) {
        return DessinBlanc(img1);
    }
    return Incluse(img1->Im[0], img2->Im[0]) &&
           Incluse(img1->Im[1], img2->Im[1]) &&
           Incluse(img1->Im[2], img2->Im[2]) &&
           Incluse(img1->Im[3], img2->Im[3]);
}

/********************************************************/
/*                                                      */
/*           CompteSousImagesGrises()                   */
/*                                                      */
/********************************************************/
// L'idée est la même que pour QuotaNoir,
// c-a-d les résultats des sous-images sont combinés 
// pour obtenir le résultat final.
bool estGrise(image img) {
    float q = QuotaNoir(img);
    return 1.f/3 <= q && q <= 2.f/3;
}
int CompteSousImagesGrises(image img) {
    if (img == NULL || img->blanc == true) {
        return 0;
    }

    int cnt = estGrise(img) ? 1 : 0; 
    for (int i = 0; i < 4; i++) {
        cnt += CompteSousImagesGrises(img->Im[i]);
    }
    return cnt;
}

/********************************************************/
/*                                                      */
/*                      Labyrinthe()                    */
/*                                                      */
/********************************************************/

/* Nous abordons cette question en convertissant l'image codée en quadtree
 * en tableau 2D, puis en effectuant un DFS (parcours en profondeur)
 * dessus afin de trouver le chemin entre deux points. 
 * Cela nous donne une complexité O(N^2). 
 * Le BFS (parcours en largeur) fonctionnerait également, 
 * mais comme nous ne cherchons pas à trouver le chemin le plus court, 
 * un DFS récursif classique suffit.
 *
 * Nous n'avons pas envisagé concrètement la méthode alternative 
 * consistant à parcourir directement le quadtree pour trouver le chemin, 
 * car cela semble assez complexe. Nous supposons qu'une telle approche 
 * donnerait un temps d'exécution meilleur que O(N^2) 
 * si le labyrinthe est "creuse".
 *
 * Pour convertir une image quadtree en tableau 2D, 
 * on utilise le même principe que la fonction Dessine(img) 
 * pour dessiner la représentation de l'image sur le terminal.
 * Au lieu d'imprimer chaque caractère ligne par ligne, on ajoute 
 * chaque caractère d'une ligne au tableau row, et le tableau 2D 
 * est formée par ces tableaux.
 *
 **************************************************************/

// convertLine remplit la ligne y du tableau row
// cur_idx est l'index courant dans row
// Cf. la fonction dessineLigne() pour plus de détails de l'explication
void convertLine(image img, int* cur_idx, int* row, int y, int p, int n) {
    if (img == NULL) {
        for (int i = 0; i < n; i++) {
            row[(*cur_idx)++] = 1; 
        }
        return;
    }
    if (img->blanc) {
        for (int i = 0; i < n; i++) {
            row[(*cur_idx)++] = 0;
        }
        return;
    }
    
    int n2 = n / 2;
    if (y < n2) {  // top
        convertLine(img->Im[0], cur_idx, row, y, p - 1, n2);
        convertLine(img->Im[1], cur_idx, row, y, p - 1, n2);
    } else {  // bottom
        convertLine(img->Im[2], cur_idx, row, y - n2, p - 1, n2);
        convertLine(img->Im[3], cur_idx, row, y - n2, p - 1, n2);
    }
}
/********************************************************************/
// Convertit l'image img en tableau 2D arr de taille N*N
// où N = 2^p et p est la profondeur de img
// en utilisant la fonction auxiliaire convertLine()
// pour remplir chaque ligne
void convertToMatrix(image img, int N, int arr[N][N]) {
    int p = Profondeur(img);
    for (int i = 0; i < N; i++) {
        int cur_idx = 0;
        convertLine(img, &cur_idx, arr[i], i, p, N);
    }
}

/********************************************************************/
// Vérifie si les coordonnées (x, y) sont valides dans le tableau N*N
bool checkValid(int x, int y, int N) {
    return (x >= 0 && x < N && y >= 0 && y < N);
}

/********************************************************************/
// Parcours en profondeur (DFS) pour trouver un chemin
// de (x, y) à (N-1, N-1)
// Le tableau 2D visited est pour marquer les cellules visitées
// dirX et dirY sont les déplacements possibles (droite, bas, gauche, haut)
bool DFS(int x, int y, int N, int arr[N][N], bool visited[N][N], int dirX[4], int dirY[4])  {
    // cas de base: atteint le point (N-1, N-1)
    if (x == N - 1 && y == N - 1) {
        return true; 
    }
    
    visited[x][y] = true;

    // pour chaque direction possible
    for (int i = 0; i < 4; i++) {
        int newX = x + dirX[i];
        int newY = y + dirY[i];

        // si la nouvelle position est valide, non visitée et pas un mur (0 = blanc)
        if (checkValid(newX, newY, N) && arr[newX][newY] == 0 && !visited[newX][newY]) {
            // appel récursif, si un chemin est trouvé, renvoie true tout de suite
            if (DFS(newX, newY, N, arr, visited, dirX, dirY)) {
                return true;
            }
        }
    }
    return false;
}
/********************************************************************/

bool Labyrinthe(image img) {
    int p = Profondeur(img);
    int N = 1 << p;
    int arr[N][N];
    
    // convertir l'image en tableau 2D
    convertToMatrix(img, N, arr);
    
    // les déplacements possibles (droite, bas, gauche, haut)
    int dirX[] = {1, 0, -1, 0};
    int dirY[] = {0, 1, 0, -1};
   
    // init visited à false
    bool visited[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            visited[i][j] = false;
        }
    }

    return DFS(0, 0, N, arr, visited, dirX, dirY);
}

/********************************************************************/
/********************************************************/
/*                                                      */
/*                      TEST()                          */
/*                                                      */
/********************************************************/

void TEST() {
    // Q5
    image imgQ5 = Image("**o*oooo*ooooooo*oooo");
    assert(DessinBlanc(imgQ5) == true);
    assert(DessinNoir(imgQ5) == false);
    FreeImg(imgQ5);

    // Q6
    image imgQ6 = Image("*Z*oZooZ*ZZZo");
    assert(QuotaNoir(imgQ6) == 0.75f);
    FreeImg(imgQ6);

    // Q7
    image imgQ7 = Image("***ooZo**ZZoooZZ*Zoo*ZooZZ ***ooZZoZZ*o*ooZoo*Zooo*oZ*oooZo*Z*oZoZoo *Z**ooZoooZZ*oooZ **oZZZZ*oooZ*oZoo");
    image imgQ7_copy = Copie(imgQ7);
    assert(equals(imgQ7, imgQ7_copy) == true);
    FreeImg(imgQ7);
    // liberer l'image originale n'influence pas la copie
    // -> faut pas SegFault
    FreeImg(imgQ7_copy);

    // Q8
    image imgQ8 = Diagonale(3);
    image ans_imgQ8 = Image("***ZooZoo*ZooZoo**ZooZoo*ZooZ");
    assert(equals(imgQ8, ans_imgQ8) == true);
    FreeImg(imgQ8);
    FreeImg(ans_imgQ8);

    // Q9
    image imgQ9 = Image("* (*ZZZZ) (*Zo(*Z(*Z(*ZZZZ)(*ZZZZ)(*ZZZZ))ZZ)o) (*ZoZ(*ZoZ(*oooo))) (*oo(*oooo)o)");
    image ans_imgQ9 = Image("*(*ZZZZ)(*ZoZo)(*ZoZ(*ZoZ(*oooo)))(*oooo)");
    SimplifieProfP(&imgQ9, 2);
    assert(equals(imgQ9, ans_imgQ9) == true);
    FreeImg(imgQ9);
    FreeImg(ans_imgQ9);

    // Q10
    image imgQ10_0 = Image("***ooooZoZoZ**ooZZoo*ZooZ");
    image imgQ10_1 = Image("**oZZZ*ooZo*ZZZZ*ZoZ*ZZZo");
    assert(Incluse(imgQ10_0, imgQ10_1) == false);

    // On change le dernier pixel tout en bas à droite de imgQ10_0 de noir à blanc
    image imgQ10_2 = Image("***ooooZoZoZ**ooZZoo*Zooo");
    assert(Incluse(imgQ10_2, imgQ10_1) == true);
    assert(Incluse(imgQ10_2, imgQ10_0) == true);
    
    FreeImg(imgQ10_0);
    FreeImg(imgQ10_1);
    FreeImg(imgQ10_2);

    // Q11
    image imgQ11_0 = Image("*Z*oZooo*Zooo");
    assert(CompteSousImagesGrises(imgQ11_0) == 1);
     
    image imgQ11_1 = Image("*oZ*Z*oZooo*Zooo *Z*oZooo*ZZoo");
    assert(CompteSousImagesGrises(imgQ11_1) == 4);
 
    image imgQ11_2 = Image("**o*oooo*ooooooo*oooo");
    assert(CompteSousImagesGrises(imgQ11_2) == 0);

    FreeImg(imgQ11_0);
    FreeImg(imgQ11_1);
    FreeImg(imgQ11_2);

    // Q12
    image imgQ12_0 = Image("***ooZo**ZZoooZZ*Zoo*ZooZZ ***ooZZoZZ*o*ooZoo*Zooo*oZ*oooZo*Z*oZoZoo *Z**ooZoooZZ*oooZ **oZZZZ*oooZ*oZoo");
    assert(Labyrinthe(imgQ12_0) == true);

    image imgQ12_1 = Image("***ooZo**ZZoooZZ*Zoo*ZooZZ ***ooZZoZZ*o*ooZoo*Zooo*oZ*ZooZo*Z*oZoZoo *Z**ooZoooZZ*oooZ **oZZZZ*oooZ*oZoo");
    assert(Labyrinthe(imgQ12_1) == false);
    
    FreeImg(imgQ12_0);
    FreeImg(imgQ12_1);
    
    printf("ALL TESTS PASSED!\n");
}

int main() {
    TEST();

    image img = Image("***ooZo**ZZoooZZ*Zoo*ZooZZ ***ooZZoZZ*o*ooZoo*Zooo*oZ*oooZo*Z*oZoZoo *Z**ooZoooZZ*oooZ **oZZZZ*oooZ*oZoo");
    Dessine(img); 
    printf("Traversable? : %s\n", Labyrinthe(img) ? "Yes" : "No");

    FreeImg(img);
    return 0;
}

