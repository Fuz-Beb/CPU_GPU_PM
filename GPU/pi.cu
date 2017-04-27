/*
This program will numerically compute the integral of
                  4/(1+x*x)
from 0 to 1.  The value of this integral is pi -- which
is great since it gives us an easy way to check the answer.
The is the original sequential program.  It uses the timer
from the OpenMP runtime library
History: Written by Tim Mattson, 11/99.
*/

#include <stdlib.h>
#include "omp_repair.h"
#include "book.h"

const long num_steps = 100000000;
const int nbBlocks = 32;
const int nbThreadPerBlock = 1024;


__global__ void kernel(double step, double * GPU) {

    int tid = threadIdx.x + blockIdx.x * blockDim.x, i;
    double result = 0.0, x;

    for (i = tid; i <= num_steps ; i += nbThreadPerBlock * nbBlocks) {
        x = (i-0.5)*step;
        result = result + 4.0/(1.0+x*x);
    }
    
    // Affectation du resultat d'un seul thread
    GPU[tid] = result;
}

int main ()
{
    double pi, start_time, run_time, step, final = 0, * dev_Sum, * sum;
    
    sum = (double*) malloc((nbThreadPerBlock * nbBlocks) * sizeof(double));

    step = 1.0/(double) num_steps;
    
    start_time = omp_get_wtime();
    
    HANDLE_ERROR(cudaMalloc((void**)&dev_Sum, sizeof(double) * (nbBlocks * nbThreadPerBlock)));
    kernel<<<nbBlocks, nbThreadPerBlock>>>(step, dev_Sum);
    HANDLE_ERROR(cudaMemcpy(sum, dev_Sum, sizeof(double) * (nbBlocks * nbThreadPerBlock), cudaMemcpyDeviceToHost));

    for (int i = 0; i < nbBlocks * nbThreadPerBlock; i++) {
        final += sum[i];
    }
    
    pi = step * final;
    
    run_time = omp_get_wtime() - start_time;
    printf("\n pi with %ld steps is %lf in %lf seconds\n ",num_steps,pi,run_time);
}