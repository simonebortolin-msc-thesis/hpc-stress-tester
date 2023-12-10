//
// Created by simon on 07/12/2023.
//
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <stdint-gcc.h>
#include "hpc_matrix.h"

void md(long row, long col, time_t duration) {
    hpc_matrix<double> dm1(row, col, [](int row, int cols) { return row*10.0 + cols*0.1; });
    hpc_matrix<double> dm2(row, col, [](int row, int cols) { return row*0.1 + cols*10.0; });
    hpc_matrix<double> dm3(row, col, [row](int, int) { return (rand() % row)*1.0; });

    time_t start_time = time(nullptr);

    while (time(nullptr) - start_time <= duration) {
        hpc_matrix<double> result = ((dm1+dm2)*(dm1+dm2)).transpose();
        double det = result.determinant();
        result = result.power(4);
        if(det > 1) result*=det;

        for (size_t i = 0; i < result.getRows(); ++i) {
            for (size_t j = 0; j < result.getCols(); ++j) {
                result(i, j) += rand() % 100;
            }
        }

        det = result.determinant();
        if(det > 1) result*=det;
        result += result;
        result -= result;
        result *= 2;

        for(int j = 0; j < 100 && dm3.determinant() == 0; j++) {
            dm3.initialize([row](int, int) { return (rand() % row)*1.0; });
        }

        hpc_matrix<double> inverse = dm3.inverse();
        //char * buffer = new char[row * row * col * col];
        //inverse.to_string(buffer, row * row * col * col);
        //printf("%s", buffer);
        hpc_matrix<double> id = dm3 * inverse;
    }
    }

void ld(long row, long col, time_t duration) {
    hpc_matrix<long long int> dm1(row, col, [](int row, int cols) { return row*100 + cols; });
    hpc_matrix<long long int> dm2(row, col, [](int row, int cols) { return row + cols*100; });
    hpc_matrix<long long int> dm3(row, col, [row](int, int) { return (rand() % row); });

    time_t start_time = time(nullptr);

    while (time(nullptr) - start_time <= duration) {
        hpc_matrix<long long int> result = ((dm1+dm2)*(dm1+dm2)).transpose();
        long long int det = result.determinant();
        result = result.power(4);
        if(det > 1) result*=det;

        for (size_t i = 0; i < result.getRows(); ++i) {
            for (size_t j = 0; j < result.getCols(); ++j) {
                result(i, j) += rand() % 100;
            }
        }

        det = result.determinant();
        if(det > 1) result*=det;
        result += result;
        result -= result;
        result *= 2;

        for(int j = 0; j < 100 && dm3.determinant() == 0; j++) {
            dm3.initialize([row](int, int) { return (rand() % row); });
        }

        hpc_matrix<long long int> inverse = dm3.inverse();
        //char * buffer = new char[row * row * col * col];
        //inverse.to_string(buffer, row * row * col * col);
        //printf("%s", buffer);
        hpc_matrix<long long int> id = dm3 * inverse;
    }
}


void matrix(int argc, char *argv[]) {
    if(argc != 5 || strcmp(argv[1], "-m") != 0) {
        fprintf(stderr, "Usage: %s -m [duration] [rows] [cols]\n", argv[0]);
        return;
    }

    char *endptr;
    time_t duration = strtol(argv[2], &endptr, 10);

    if (*endptr != '\0') {
        fprintf(stderr, "Invalid duration: %s\n", argv[2]);
        return;
    }

    long row = strtol(argv[3], &endptr, 10);

    if (*endptr != '\0') {
        fprintf(stderr, "Invalid row size: %s\n", argv[3]);
        return;
    }

    long col = strtol(argv[4], &endptr, 10);

    if (*endptr != '\0') {
        fprintf(stderr, "Invalid col size: %s\n", argv[3]);
        return;
    }

    md(row, col, duration/2);
    ld(row, col, duration/2);
}
