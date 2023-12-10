//
// Created by simon on 07/12/2023.
//
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

void pi(int argc, char *argv[]) {
    if(argc != 4 || strcmp(argv[1], "-p") != 0) {
        fprintf(stderr, "Usage: %s -p [duration] [size]\n", argv[0]);
        return;
    }

    char *endptr;
    const time_t duration = strtol(argv[2], &endptr, 10);

    if (*endptr != '\0') {
        fprintf(stderr, "Invalid duration: %s\n", argv[2]);
        return;
    }

    const long size = strtol(argv[3], &endptr, 10);

    if (*endptr != '\0') {
        fprintf(stderr, "Invalid terms: %s\n", argv[3]);
        return;
    }

    const time_t start_time = time(nullptr);

    while (time(nullptr) - start_time <= duration) {
        double pi = 0.0;
        int sign = 1;

        for (int i = 0; i < size; ++i) {
            pi += static_cast<double>(sign) / (2 * i + 1);
            sign = -sign;
        }
    }
}