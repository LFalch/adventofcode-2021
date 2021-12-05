#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <tgmath.h>
#include <assert.h>
#define _GNU_SOURCE 1

#define sgn(x) ((x > 0) - (x < 0))

int main() {
    FILE *f = fopen("input.txt", "r");

    int vents[1000][4];
    size_t vents_n = 0;

    char *line;
    size_t n;

    while (getline(&line, &n, f) > 0) {
        sscanf(line, "%d,%d -> %d,%d", &vents[vents_n][0], &vents[vents_n][1], &vents[vents_n][2], &vents[vents_n][3]);
        vents_n++;
    }

    unsigned short grid[1000][1000] = { 0 };
    unsigned points_quick = 0;

    for (size_t i = 0; i < vents_n; i++) {
        int x1 = vents[i][0], y1 = vents[i][1], x2 = vents[i][2], y2 = vents[i][3];
        if (x1 == x2) {
            if (y1 > y2) {
                int temp = y1;
                y1 = y2;
                y2 = temp;
            }
            for (int y = y1; y <= y2; y++) {
                grid[x1][y] += 1;
                if (grid[x1][y] == 2) points_quick++;
            }
        } else if (y1 == y2) {
            if (x1 > x2) {
                int temp = x1;
                x1 = x2;
                x2 = temp;
            }
            for (int x = x1; x <= x2; x++) {
                grid[x][y1] += 1;
                if (grid[x][y1] == 2) points_quick++;
            }
        }
        #ifdef PART2
        else {
            if (x1 > x2) {
                int temp = x1, temp2 = y1;
                x1 = x2;
                y1 = y2;
                x2 = temp;
                y2 = temp2;
            }

            int dy = sgn(y2 - y1);
            int x = x1, y = y1;

            while (x <= x2) {
                grid[x][y] += 1;
                if (grid[x][y] == 2) points_quick++;

                x++; y += dy;
            }
        }
        #endif
    }
    printf("%u\n", points_quick);
}
