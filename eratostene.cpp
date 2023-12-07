//
// Created by simon on 07/12/2023.
//
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <stdint-gcc.h>


void eratostene(int argc, char *argv[]) {
    if(argc != 4 || strcmp(argv[1], "-e") != 0) {
        fprintf(stderr, "Usage: %s -e [duration] [size]\n", argv[0]);
        return;
    }

    char *endptr;
    time_t duration = strtol(argv[2], &endptr, 10);

    if (*endptr != '\0') {
        fprintf(stderr, "Invalid duration: %s\n", argv[2]);
        return;
    }

    size_t size = strtol(argv[3], &endptr, 10);

    if (*endptr != '\0') {
        fprintf(stderr, "Invalid size: %s\n", argv[3]);
        return;
    }

    auto * matrix = new uint8_t [size];

    time_t start_time = time(NULL);

    while (time(NULL) - start_time <= duration) {
        for(size_t i = 0; i < size; i++) {
            matrix[i] = 1;
        }

        for(size_t i = 2; i < size; i++) {
            if(matrix[i]) {
                for(size_t j=2*i; j < size; j += i) {
                    matrix[j] = 0;
                }
            }
        }
    }
    delete[] matrix;
}