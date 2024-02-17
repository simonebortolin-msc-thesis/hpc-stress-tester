/*
* References:
	* https://software.intel.com/en-us/articles/intel-advanced-vector-\extensions-512-intel-avx-512-new-vector-neural-network-instruction
	* https://en.wikichip.org/wiki/x86/avx512vnni
*/

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

#define MAXRAND 10

__m128i A,B,result;

void foo(int a, int b, int c){

	uint8_t arr_a[NUM_8B_INT_IN_M128];
	uint8_t arr_b[NUM_8B_INT_IN_M128];

    for (int i=0; i<NUM_8B_INT_IN_M128; i++){
        arr_a[i] = (uint8_t)a;
        arr_b[i] = (uint8_t)b;
	}


	A = _mm_loadu_si128((__m128i*)&arr_a);
	B = _mm_loadu_si128((__m128i*)&arr_b);

	/*
	Vertically multiply each unsigned 8-bit integer from a with the
	corresponding signed 8-bit integer from b, producing intermediate signed
	16-bit integers. Horizontally add adjacent pairs of intermediate signed
	16-bit integers, and pack the saturated results in dst.
	*/
	result = _mm_maddubs_epi16(A,B);

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

    if(argc != 3 || strcmp(argv[1], "-s") != 0) {
        fprintf(stderr, "Usage: %s -s [duration]\n", argv[0]);
        return 1;
    }

    char *endptr;
    time_t duration = strtol(argv[2], &endptr, 10);

    if (*endptr != '\0') {
        fprintf(stderr, "Invalid duration: %s\n", argv[2]);
        return 1;
    }

    time_t start_time = time(NULL);

    while (time(NULL) - start_time <= duration)
    {
        srand((unsigned)time(NULL));
        int a =  (rand() % MAXRAND) + 1;
        int b =  (rand() % MAXRAND) + 1;
        int c =  (rand() % MAXRAND) + 1;

        foo(a,b,c);
    }
    //if(f!=0) MPI_Finalize();
    MPI_Finalize();
    return 0;
}