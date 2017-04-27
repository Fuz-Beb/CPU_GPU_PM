#include <stdio.h>
#include <stdlib.h>
#include "omp_repair.h"

#define LigneA 400
#define ColonneA 400
#define ColonneB 400

int main (int argc, char *argv[])
{
int	tid, nthreads, i, j, k, chunk;
double	a[LigneA][ColonneA],	// Matrice A a multiplier
	b[ColonneA][ColonneB],      // Matrice B a multiplier
	c[LigneA][ColonneB];        // Matrice de resultat

double start_time = 0, run_time = 0; // Pour le compteur

chunk = 10; // set loop iteration chunk size



/*** Spawn a parallel region explicitly scoping all variables ***/
#pragma omp parallel shared(a,b,c,nthreads,chunk) private(tid,i,j,k)
  {
  tid = omp_get_thread_num();
  if (tid == 0)
    {
    nthreads = omp_get_num_threads();
    printf("Starting matrix multiple example with %d threads\n",nthreads);
    printf("Initializing matrices...\n");
    }
  /*** Initialisation des matrices A B et C ***/
  #pragma omp for schedule (static, chunk)
  for (i=0; i<LigneA; i++)
    for (j=0; j<ColonneA; j++)
      a[i][j]= i+j;
  #pragma omp for schedule (static, chunk)
  for (i=0; i<ColonneA; i++)
    for (j=0; j<ColonneB; j++)
      b[i][j]= i*j;
  #pragma omp for schedule (static, chunk)
  for (i=0; i<LigneA; i++)
    for (j=0; j<ColonneB; j++)
      c[i][j]= 0;

start_time = omp_get_wtime(); // Donne le debut de l'exécution en sec

  /*** Do matrix multiply sharing iterations on outer loop ***/
  /*** Display who does which iterations for demonstration purposes ***/
  printf("Thread %d exécute la multiplication de matrice...\n",tid);
  #pragma omp for schedule (static, chunk)
  for (i=0; i<LigneA; i++)
    {
    printf("Thread=%d a fait la ligne numéro =%d\n",tid,i);
    for(j=0; j<ColonneB; j++)
      for (k=0; k<ColonneA; k++)
        c[i][j] += a[i][k] * b[k][j];
    }
  }   /*** End of parallel region ***/

run_time = omp_get_wtime() - start_time;

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
