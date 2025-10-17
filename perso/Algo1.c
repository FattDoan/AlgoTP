

/****************** monôme rosaz@lisn.fr       ****/

#include <stdio.h>
#include <stdlib.h>

/*************************************************/
/*                                               */
/*                type booléen                   */
/*                                               */
/*************************************************/

typedef enum {false, true} bool;

/*************************************************/
/*                                               */
/*            factorielle                        */
/*                                               */
/*************************************************/

long fact1 (int n) 
{ if (n==0) return 1 ;
  else return fact1(n-1) * n ; 
}

/*************************************************/

// itou avec un argument out => passage par adresse
             // le calcul se fait comme dans la version récursive
             // (...(((1*1)*2)*3) ... *n)
void bisfact2(int n, long * r) 
{ if (n==0) 
         *r = 1 ;
  else { bisfact2(n-1,r) ;
         *r = n**r ;  // ou *r*n
       }        // Notez le double sens de * ...
}

long fact2 (int n)
{ long r ;
  bisfact2(n,&r) ;
  return r ;
}

/*************************************************/

// variante qui consiste en fait à initialier r beaucoup plus tôt
// du coup, r est inout

void bisfact3(int n, long * r) 
{ if (n==0) {}
  else { bisfact3(n-1,r) ;
         *r = n**r ;  }  
}

long fact3 (int n)
{ long r = 1 ;
  bisfact3(n,&r) ;
  return r ;
}

/*************************************************/

// Ici r est vraiment inout, récursif terminal
// le calcul est différent : (((...((1*n)*(n-1)) ... *3)*2)*1)

void bisfact4(int n, long * r) 
{ if (n==0) {}
  else { *r = n**r ;  
        bisfact4(n-1,r) ; }  
}

long fact4 (int n)
{ long r = 1 ;
  bisfact4(n,&r) ;
  return r ;
}

/*************************************************/

// et ce code-ci est *** FAUX **** !!! Comprenez-vous pourquoi ?

void bisfact5(int n, long * r) 
{ if (n==0) *r = 1 ;
  else { *r = n**r ;  
        bisfact5(n-1,r) ; }  
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

#define VersionsFact 6
     // nombre de versions de factorielle

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


/*************************************************/
/*                                               */
/*            Calcul de e                        */
/*                                               */
/*************************************************/

  // d'après google,
  // e = 2,7182818284 5904523536 0287471352 6624977572 4709369995 
  //       9574966967 6277240766 3035354759 4571382178 5251664274
  // il est bien sûr interdit d'injecter directement cette approximation

     // Il est proposé de faire 3 versions pour observer des phénomènes de précision.
     // C'est le même code, seul le type change.


float Efloat ()
{ return 0 ; }

/*************************************************/

double Edouble ()
{ return 0 ; }
  
/*************************************************/

long double Elongdouble ()
{ return 0 ; }

/*************************************************/
/*                                               */
/*            Suite Y                            */
/*                                               */
/*************************************************/

void afficheYfloat (int n) 
{ }

/*************************************************/

void afficheYdouble (int n)  
{ }

/*************************************************/

void afficheYlongdouble (int n)
{ }

/*************************************************/
/*                                               */
/*          Permutations tableaux                */
/*                                               */
/*************************************************/

int Int_Lire ()     // Lecture simpliste d'un entier positif ou nul
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
 
/*************************************************/

int* P_Lire (int n)
{ printf("Input Permutation 0..%d : ", n-1) ;
int* T = (int*) malloc(n*sizeof(int)) ;
  for (int i=0 ; i<n ; i++) T[i] = Int_Lire() ;
  return T ;
}

/*************************************************/

int* P_Zero (int n)    // Permutation bidon, qui n'est pas une permutation (sauf n=0 ou 1 ...)
{ int* T = (int*) malloc(n*sizeof(int)) ;
  for (int i=0 ; i<n ; i++) T[i] = 0 ;
  return T ;
}

/*************************************************/

void P_Affiche (int* P , int n)
{ printf("[") ;
  for (int i=0 ; i<n ; i++) printf(" %d",P[i]) ;
  printf(" ]\n") ;
} 

/*************************************************/

int* P_identite (int n)
{ return P_Zero(n) ; }

/*************************************************/

int* P_Inverse (int* P , int n)
{ return P_Zero(n) ; }

/*************************************************/

void P_Compose(int* P, int* Q, int* R , int n)  // écrit PoQ dans R
{ }

/*************************************************/

bool P_Verifie (int* P , int n)
{ return true ; }


/**********************/

int* P_power1(int* P, int n, int k) // itératif, complexité environ k
{ return P_Zero(n) ; }

/**********************/

int* P_power2(int* P, int n, int k) // récursif, complexité environ k
{ return P_Zero(n) ; }

/**********************/

int* P_power3(int* P, int n, int k) // récursif, complexité environ log2(k)  
{ return P_Zero(n) ; }

/**********************/

int* P_power4(int* P, int n, int k) // itératif, complexité environ log2(k)  
{ return P_Zero(n) ; }

/*************************************************/

const int VersionsPuissance = 4 ;
    // nombre de versions de Puissance, soit 4 selon énoncé

/*************************************************/

int* P_power(int* P, int n, int k, int v )   // version v de 1 à VersionsPuissance
{ switch (v)  
   {
   case 1 : return P_power1(P,n,k) ; break ;
   case 2 : return P_power2(P,n,k) ; break ;
   case 3 : return P_power3(P,n,k) ; break ;
   case 4 : return P_power4(P,n,k) ; break ;
   default : return NULL ; 
   }
}

/*************************************************/

int* P_random (int n) 
{ return P_identite(n) ; 
}


/*************************************************/
/*                                               */
/*            Syracuse                           */
/*                                               */
/*************************************************/

        /**********************/
        
#define CSyr 2025
      
        /**********************/
        

/*************************************************/

int SyracuseI (int n)
{ return 0 ; }
  
/*************************************************/

int SyracuseSF (int n)
{ return 0 ; }

/*************************************************/

int SyracuseSP (int n)
{ return 0 ; }

/*************************************************/

int SyracuseR (int n)
{ return 0 ; }

 
/*************************************************/

#define VersionsSyracuse 4
    // nombre de versions codees

/*************************************************/

int Syracuse (int n, int i)
{ switch (i)  
   {
   case 1 : return SyracuseI  (n) ; // break ;  
   case 2 : return SyracuseSF (n) ; // break ;  
   case 3 : return SyracuseSP (n) ; // break ;  
   case 4 : return SyracuseR  (n) ; // break ;  
   default : return 0 ; 
   }
}



/*************************************************/
/*                                               */
/*               main                            */
/*                                               */
/*************************************************/


// int main(int argc, char** argv)

 int main()
{
       

       
/************************************************************************/
 
      // mettre "if true" vs "if false" selon que vous voulez les tests ou pas
  
/************************  taille des nombres  *************************/
      
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

if (false) {  

        printf(" e1 = %.20f  \n", Efloat() ) ;
        printf(" e3 = %.30lf \n", Edouble() ) ; 
        printf(" e3 = %.40LF \n", Elongdouble() ) ; 
        printf("\n") ;

        
}

if (false) {  
            printf("Valeurs de Y, selon float, double, longdouble :\n") ;
            afficheYfloat(30) ;
            afficheYdouble(30) ;
            afficheYlongdouble(30) ;
            printf("\n") ;
}
 

/******************************* Permutations **************************/

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



/***********************************************************************/

 

    return 0;
}






