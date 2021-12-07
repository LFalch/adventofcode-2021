#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#define _GNU_SOURCE 1

int main() {
    FILE *f = fopen("input.txt", "r");

    size_t crabs_n = 0;
    int crabs[10000];

    int maxPos = -1;
    int crab;
    char c;
    while (fscanf(f, "%d%c", &crab, &c) > 0) {
        crabs[crabs_n++] = crab;
        if (crab > maxPos) maxPos = crab;
        if (c != ',') break;
    }

    unsigned fuel_cons_min = UINT_MAX;

    for (int p = 0; p <= maxPos; p++) {
        unsigned fuel_cons = 0;
        for (size_t i = 0; i < crabs_n; i++) {
            fuel_cons += abs(crabs[i] - p);
        }
        if (fuel_cons < fuel_cons_min) fuel_cons_min = fuel_cons;
    }
    printf("%u\n", fuel_cons_min);

    fuel_cons_min = UINT_MAX;

    for (int p = 0; p <= maxPos; p++) {
        unsigned fuel_cons = 0;
        for (size_t i = 0; i < crabs_n; i++) {
            unsigned diff = abs(crabs[i] - p);
            fuel_cons += (diff * (diff + 1)) / 2;
        }
        if (fuel_cons < fuel_cons_min) fuel_cons_min = fuel_cons;
    }
    printf("%u\n", fuel_cons_min);
}
