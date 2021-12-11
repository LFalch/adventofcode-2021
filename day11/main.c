#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#define _GNU_SOURCE 1

#define inbounds(i, j) ((i) >= 0 && (j) >= 0 && (i) < 10 && (j) < 10)

void light_surroundings(ssize_t i, ssize_t j, short points[10][10]) {
    if (points[i][j] == 10) {
        for (ssize_t i_off = -1; i_off <= 1; i_off++) {
            for (ssize_t j_off = -1; j_off <= 1; j_off++) {
                if (i_off == 0 && j_off == 0)
                    continue;
                else if (inbounds(i + i_off, j + j_off)) {
                    points[i + i_off][j + j_off]++;
                    light_surroundings(i + i_off, j + j_off, points);
                }
            }
        }
    }
}

int main() {
    FILE *f = fopen("input.txt", "r");

    size_t n_areas = 0, cur_point = 0;
    short points[10][10];
    char c;
    while (fread(&c, 1, 1, f) == 1) {
        if (c == '\n') {
            n_areas++;
            cur_point = 0;
        } else {
            points[n_areas][cur_point] = c - '0';

            cur_point++;
        }
    }

    size_t flashes = 0, step_flashes = 0;
    unsigned long nc;

    for (nc = 0; step_flashes < 100; nc++) {
        for (ssize_t i = 0; i < 10; i++) {
            for (ssize_t j = 0; j < 10; j++) {
                points[i][j]++;

                light_surroundings(i, j, points);
            }
        }

        step_flashes = 0;
        for (size_t i = 0; i < 10; i++) {
            for (size_t j = 0; j < 10; j++) {
                if (points[i][j] > 9) {
                    step_flashes++;
                    points[i][j] = 0;
                }
            }
        }
        flashes += step_flashes;
        if (nc == 99) {
            printf("%zu\n", flashes);
        }
    }
    printf("%lu\n", nc);
}
