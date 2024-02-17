#include <stdio.h>
#include <immintrin.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint-gcc.h>
#include <mpi.h>
#include "common.h"


float a[256] = {0};
float b[256] = {0};
float c[256] = {0};

void foo(){
    __m256 result,B,C;
        for (int i=0; i<256; i+=sizeof(__m256)/sizeof(float)){
            B =  _mm256_load_ps(&b[i]);
            C =  _mm256_load_ps(&c[i]);
            result = _mm256_add_ps(B,C);
            _mm256_store_ps(&a[i], result);
        }
}

void fill_arrays(){
    for (int i=0; i<256; i++){
        b[i] = 1.0;
        c[i] = 2.0;

    }
}

int check_arrays(){
    int ret = 0;
    for (int i=0; i<256; i++){
        if (a[i] == 3)
            continue;
        else
            printf("FAIL, corruption in arithmetic");
            ret =  -1;
            break;
    }
    return ret;
}


int main(int argc, char *argv[]){
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if(argc != 3 || strcmp(argv[1], "-a") != 0) {
        fprintf(stderr, "Usage: %s -a [duration]\n", argv[0]);
        return 1;
    }

    char *endptr;
    time_t duration = strtol(argv[2], &endptr, 10);

    if (*endptr != '\0') {
        fprintf(stderr, "Invalid duration: %a\n", argv[2]);
        return 1;
    }

    time_t start_time = time(NULL);

    while (time(NULL) - start_time <= duration)
    {
        // initialize arrays
        fill_arrays();
        foo();
        //if (check_arrays())
        //    return -1;
        //printf("Works !!!\n");
    }
    //if(f!=0) MPI_Finalize();
    MPI_Finalize();
    return 0;
}