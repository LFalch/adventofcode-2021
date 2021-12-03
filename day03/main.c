#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#define _GNU_SOURCE 1

int main() {
    FILE *f = fopen("input.txt", "r");

    char *s = NULL;
    size_t n = 0;
    int *bit_freq = NULL;
    unsigned lines = 0;
    ssize_t len;
    size_t length;

    while ((len = getline(&s, &n, f)) > 0) {
        if (bit_freq == NULL) {
            length = len;
            bit_freq = calloc(length, sizeof(int));
        }
        size_t i;
        for (i = 0; ((unsigned) (s[i] - '0')) < 2; i++) {
            bit_freq[i] += s[i] - '0';
        }
        length = i;
        lines++;
    }
    printf("%d\n", lines);
    for (size_t i = 0; i < length; i++) {
        printf("%d ", bit_freq[i]);
    }
    printf("\n");
    unsigned epsilon = 0, gamma = 0;
    for (int i = 0; i < length; i++) {
        int bit = (bit_freq[length - i - 1] > (lines / 2)) ? 1 : 0;
        gamma |= (bit << i);
        bit = bit ? 0 : 1;
        epsilon |= (bit << i);
    }

    printf("e: %x\n", epsilon);
    printf("g: %x\n", gamma);
    printf("%d\n", epsilon * gamma);

    free(bit_freq);
    fclose(f);
    f = fopen("input.txt", "r");

    unsigned *o2_nums, *co2_nums;
    o2_nums = malloc(lines * sizeof(unsigned));
    co2_nums = malloc(lines * sizeof(unsigned));
    size_t length_o2 = 0, length_co2 = 0;

    while ((len = getline(&s, &n, f)) > 0) {
        char *end;
        unsigned num = strtol(s, &end, 2);
        if (s[0] == '1') {
            o2_nums[length_o2++] = num;
        } else {
            co2_nums[length_co2++] = num;
        }
    }
    if (length_co2 > length_o2) {
        unsigned *tempp = o2_nums;
        o2_nums = co2_nums;
        co2_nums = tempp;
        size_t temp = length_o2;
        length_o2 = length_co2;
        length_co2 = temp;
    }

    for (int i = 1; i < length; i++) {
        unsigned *l0, *l1;
        l0 = malloc(length_o2 * sizeof(unsigned));
        l1 = malloc(length_o2 * sizeof(unsigned));

        size_t ones = 0, zeroes = 0;

        for (size_t j = 0; j < length_o2; j++) {
            if ((o2_nums[j] >> (length - i - 1)) & 1) {
                l1[ones++] = o2_nums[j];
            } else {
                l0[zeroes++] = o2_nums[j];
            }
        }
        free(o2_nums);
        if (ones >= zeroes) {
            o2_nums = l1;
            length_o2 = ones;
            free(l0);
        } else {
            o2_nums = l0;
            length_o2 = zeroes;
            free(l1);
        }
        printf("#o2 = %zu 1s = %zu 0s = %zu \n", length_o2, ones, zeroes);
        if (length_o2 == 1) break;
    }
    for (int i = 1; i < length; i++) {
        unsigned *l0, *l1;
        l0 = malloc(length_co2 * sizeof(unsigned));
        l1 = malloc(length_co2 * sizeof(unsigned));

        size_t ones = 0, zeroes = 0;

        for (size_t j = 0; j < length_co2; j++) {
            if ((co2_nums[j] >> (length - i - 1)) & 1) {
                l1[ones++] = co2_nums[j];
            } else {
                l0[zeroes++] = co2_nums[j];
            }
        }
        free(co2_nums);
        if (ones < zeroes || zeroes == 0) {
            co2_nums = l1;
            length_co2 = ones;
            free(l0);
        } else {
            co2_nums = l0;
            length_co2 = zeroes;
            free(l1);
        }
        printf("#co2 = %zu 1s = %zu 0s = %zu \n", length_co2, ones, zeroes);
        if (length_co2 == 1) break;
    }
    printf("o2: %d\n", o2_nums[0]);
    printf("co2: %d\n", co2_nums[0]);
    printf("%d\n", o2_nums[0] * co2_nums[0]);

    free(s);
    free(o2_nums);
    free(co2_nums);
    fclose(f);
}
