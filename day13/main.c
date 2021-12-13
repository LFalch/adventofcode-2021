#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#define _GNU_SOURCE 1

int main() {
    FILE *f = fopen("input.txt", "r");

    char c;
    int l, x, y;

    int folds[50];
    size_t folds_n = 0;

    // Rearrange input to do this
    while (fscanf(f, "fold along %c=%d\n", &c, &l) == 2) {
        folds[folds_n++] = (l << 1) | (c == 'x' ? 0 : 1);
    }

    static int coords[2000][2000] = {0};
    int coords_r[40][6] = {0};

    while (fscanf(f, "%d,%d\n", &x, &y) == 2) {
        for (size_t i = 0; i < folds_n; i++) {
            int f = folds[i];
            if (f & 1) {
                y = (f >> 1) - abs(y - (f >> 1));
            } else {
                x = (f >> 1) - abs(x - (f >> 1));
            }
            if (i == 0) {
                coords[x][y] = 1;
            }
        }
        coords_r[x][y] = 1;
    }

    int count = 0;
    for (size_t j = 0; j < 2000; j++) {
        for (size_t i = 0; i < 2000; i++) {
            if (coords[i][j] > 0) count++;
        }
    }
    printf("%d\n\n", count);
    for (size_t j = 0; j < 6; j++) {
        for (size_t i = 0; i < 40; i++) {
            if (coords_r[i][j] > 0) count++;
            printf("%c", coords_r[i][j] > 0 ? '#' : ' ');
        }
        printf("\n");
    }
}
