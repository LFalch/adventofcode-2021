#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#define _GNU_SOURCE 1
#include <assert.h>
#include <string.h>

int main() {
    FILE *f = fopen("input.txt", "r");

    unsigned long fish[9] = { 0 };

    char buf[2];
    while (fread(buf, 1, 2, f) > 0) {
        fish[buf[0] - '0'] += 1;
    }
    for (int i = 0; i < 9; i++) {
        printf("%zu ", fish[i]);
    }
    printf("\n");

    for (int d = 0; d < 80; d++) {
        unsigned long repopulating_fish = fish[0];
        memmove(&fish[0], &fish[1], 8 * sizeof(unsigned long));
        fish[6] += repopulating_fish;
        fish[8] = repopulating_fish;
    }
    size_t fish_n = 0;
    for (int i = 0; i < 9; i++) {
        fish_n += fish[i];
    }
    printf("%zu\n", fish_n);
    for (int d = 0; d < 256-80+1; d++) {
        unsigned long repopulating_fish = fish[0];
        memmove(&fish[0], &fish[1], 8 * sizeof(unsigned long));
        fish[6] += repopulating_fish;
        fish[8] = repopulating_fish;
    }
    fish_n = 0;
    for (int i = 0; i < 8; i++) {
        fish_n += fish[i];
    }
    printf("%zu\n", fish_n);
}
