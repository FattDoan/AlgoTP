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

/*******************************************************/
// function utilitaire pour liberer la mémoire
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
/*                                                      */
/*                      Wht()                           */
/*                                                      */
/********************************************************/
image Wht() {
    image img = (image)malloc(sizeof(bloc_image));
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
    img->Im[0] = i0; img->Im[1] = i1;
    img->Im[2] = i2; img->Im[3] = i3;
    if (i0 == NULL && i1 == NULL && i2 == NULL && i3 == NULL) {
        img->blanc = true;
    }
    else img->blanc = false;
    return img;
}
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

image Lecture_aux(char* arr, int* cur_idx, int n) {
    if (*cur_idx >= n) {
        return NULL;
    }
    char cur_char = arr[*cur_idx];
    (*cur_idx)++;

    // UTILIZE Cut ?
    if (cur_char == 'Z') {
        return Blk();
     }
    if (cur_char == 'o') {
        return Wht();
    }
    if (cur_char == '*') {
        image i0 = Lecture_aux(arr, cur_idx, n);
        image i1 = Lecture_aux(arr, cur_idx, n);
        image i2 = Lecture_aux(arr, cur_idx, n);
        image i3 = Lecture_aux(arr, cur_idx, n);
        return Cut(i0, i1, i2, i3);
    }
    return NULL;
}

image Lecture() {
    int c, capacity = 16, n  = 0;
    char* arr = malloc(capacity * sizeof(char));

    while ((c = getchar()) != '\n' && c != EOF) {
        // ignore non significative input
        if (c != 'o' && c != '*' && c != 'Z') continue;

        if (n + 1 >= capacity) {      // need more space
            capacity *= 2;
            char *new_arr = realloc(arr, capacity);
            arr = new_arr;
        }
        arr[n++] = c;
    }

    int cur_idx = 0;
    image res = Lecture_aux(arr, &cur_idx, n);
    free(arr);
    return res;
}
/********************************************************/
/*                                                      */
/*           DessinNoir() et DessinBlanc()              */
/*                                                      */
/********************************************************/
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
image Copie(image img) {
    if (img == NULL) return NULL;
    image new_img = (image)malloc(sizeof(bloc_image));
    new_img->blanc = img->blanc;
    for (int i = 0; i < 4; i++) {
        new_img->Im[i] = Copie(img->Im[i]);
    }
    return new_img;
}
/********************************************************/
/*                                                      */
/*                      Diagonale()                     */
/*                                                      */
/********************************************************/
image Diagonale(int p) {
    if (p == 1) {
        image img = (image)malloc(sizeof(bloc_image));
        img->blanc = false;
        img->Im[0] = Blk(); img->Im[1] = Wht(); 
        img->Im[2] = Wht(); img->Im[3] = Blk();
        return img;
    }
    

    image img = (image)malloc(sizeof(bloc_image));
    img->blanc = false;
    img->Im[0] = Diagonale(p-1); img->Im[1] = Wht();
    img->Im[2] = Wht(); img->Im[3] = Copie(img->Im[0]);

    return img;
}

int main() {
    image img = Diagonale(3);
    Affiche(img);
    printf("\n");
    ProfAffiche(img);
    printf("\n"); 
    FreeImg(img);
    return 0;
}

