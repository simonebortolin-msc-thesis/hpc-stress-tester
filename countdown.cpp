#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

void countdown(int argc, char *argv[]) {
    if(argc != 3 || strcmp(argv[1], "-c") != 0) {
        fprintf(stderr, "Usage: %s -c [duration]\n", argv[0]);
        return;
    }

    char *endptr;
    time_t duration = strtol(argv[2], &endptr, 10);

    if (*endptr != '\0') {
        fprintf(stderr, "Invalid duration: %s\n", argv[2]);
        return;
    }

    time_t start_time = time(NULL);

    while (time(NULL) - start_time <= duration);
}
