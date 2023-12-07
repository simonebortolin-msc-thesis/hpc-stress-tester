#include <unistd.h>
#include <mpi.h>
#include <cstdio>

#include "countdown.h"
#include "eratostene.h"
#include "pi.h"

int main(int argc, char *argv[]){
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int opt;

    int f = fork();

    while ((opt = getopt(argc, argv, "cep")) != -1) {
        switch (opt) {
            case 'c': countdown(argc,argv); break;
            case 'e': eratostene(argc, argv); break;
            case 'p': pi(argc, argv); break;
            default:
                fprintf(stderr, "Usage: %s [-cep] [option...]\n", argv[0]);
        }
    }

    if(f!=0) MPI_Finalize();
    return 0;
}