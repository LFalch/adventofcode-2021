#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#define _GNU_SOURCE 1

#define LINE_END "\x1b[3K\x1b[1G"
#define inbounds(i, j, n_areas, n_points) ((i) >= 0 && (j) >= 0 && (i) < n_areas && (j) < n_points)
#define ptos(i, j) (((i) << 32) | (j))
#define stop_x(s) ((s) >> 32)
#define stop_y(s) ((s) & 0xffffffff)

int in_set(size_t s, size_t *set, size_t set_length) {
    for (size_t i = 0; i < set_length; i++)
        if (s == set[i]) return 1;
    return 0;
}

int main() {
    FILE *f = fopen("input.txt", "r");

    size_t n_areas = 0, n_points = 0, cur_point = 0;
    short points[200][200];
    char c;
    while (fread(&c, 1, 1, f) == 1) {
        if (c == '\n') {
            n_areas++;
            cur_point = 0;
        } else {
            points[n_areas][cur_point] = c - '0';

            if (n_areas == 0) n_points++;
            cur_point++;
        }
    }
    long risk_level_sum = 0;
    long largest_basin = 0;
    long largest_basin2 = 0;
    long largest_basin3 = 0;
    for (size_t i = 0; i < n_areas; i++) {
        for (size_t j = 0; j < n_points; j++) {
            if (i != 0) {
                if (points[i-1][j] <= points[i][j]) { continue; }
            }
            if (j != 0) {
                if (points[i][j-1] <= points[i][j]) { continue; }
            }
            if (i != n_areas - 1 ) {
                if (points[i+1][j] <= points[i][j]) { continue; }
            }
            if (j != n_points - 1 ) {
                if (points[i][j+1] <= points[i][j]) { continue; }
            }

            risk_level_sum += 1 + points[i][j];

            size_t search_offset = 0;
            size_t search_length = 1;
            size_t search_set[10000] = {ptos(i, j)};
            long basin_size = 0;

            for (size_t search_offset = 0; search_offset < search_length; search_offset++) {
                size_t s = search_set[search_offset];
                ssize_t x = stop_x(s), y = stop_y(s);
                if (points[x][y] != 9) {
                    basin_size++;

                    if(inbounds(x-1, y, n_areas, n_points) && !in_set(ptos(x-1, y), search_set, search_length)) search_set[search_length++] = ptos(x-1, y);
                    if(inbounds(x+1, y, n_areas, n_points) && !in_set(ptos(x+1, y), search_set, search_length)) search_set[search_length++] = ptos(x+1, y);
                    if(inbounds(x, y-1, n_areas, n_points) && !in_set(ptos(x, y-1), search_set, search_length)) search_set[search_length++] = ptos(x, y-1);
                    if(inbounds(x, y+1, n_areas, n_points) && !in_set(ptos(x, y+1), search_set, search_length)) search_set[search_length++] = ptos(x, y+1);
                }
            }

            if (basin_size > largest_basin) {
                largest_basin3 = largest_basin2;
                largest_basin2 = largest_basin;
                largest_basin = basin_size;
            } else if (basin_size > largest_basin2) {
                largest_basin3 = largest_basin2;
                largest_basin2 = basin_size;
            } else if (basin_size > largest_basin3) {
                largest_basin3 = basin_size;
            }
        }
    }
    printf("%ld\n", risk_level_sum);
    printf("%ld\n", largest_basin*largest_basin2*largest_basin3);
}
