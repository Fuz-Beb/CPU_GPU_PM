#include <stdio.h>
#include <stdlib.h>
#include "omp_repair.h"

#define LigneA 50
#define ColonneA 50
#define ColonneB 50

int main (int argc, char *argv[])
{
int	i, j, k;
double	a[LigneA][ColonneA],	// Matrice A a multiplier
	b[ColonneA][ColonneB],      // Matrice B a multiplier
	c[LigneA][ColonneB];        // Matrice de resultat

double start_time = 0, run_time = 0; // Pour le compteur

/*** Spawn a parallel region explicitly scoping all variables ***/


    printf("Starting matrix multiple example\n");
    printf("Initializing matrices...\n");
  /*** Initialisation des matrices A B et C ***/

  for (i=0; i<LigneA; i++)
    for (j=0; j<ColonneA; j++)
      a[i][j]= i+j;

  for (i=0; i<ColonneA; i++)
    for (j=0; j<ColonneB; j++)
      b[i][j]= i*j;

  for (i=0; i<LigneA; i++)
    for (j=0; j<ColonneB; j++)
      c[i][j]= 0;

start_time = omp_get_wtime(); // Donne le debut de l'exécution en sec

  /*** Do matrix multiply sharing iterations on outer loop ***/
  /*** Display who does which iterations for demonstration purposes ***/
  printf("Exécutions la multiplication de matrice...\n");

  for (i=0; i<LigneA; i++)
    {
    for(j=0; j<ColonneB; j++)
      for (k=0; k<ColonneA; k++)
        c[i][j] += a[i][k] * b[k][j];
    }

run_time = omp_get_wtime() - start_time; // Calcul du temps d'execution

/*** Print results ***/
printf("******************************************************\n");
printf("Matrice de résultat :\n");
for (i=0; i<LigneA; i++)
  {
  for (j=0; j<ColonneB; j++)
    printf("%6.2f   ", c[i][j]);
  printf("\n");
  }
printf("******************************************************\n");
printf ("Terminé !\n");




printf("\n Fin d'exécution en %lf secondes\n ",run_time);

}
