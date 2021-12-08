#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#define _GNU_SOURCE 1

int popcount(unsigned char c) {
    int ret = 0;
    for (int i = 0; i < 8; i++) {
        ret += (c >> i) & 1;
    }
    return ret;
}

int main() {
    FILE *f = fopen("input.txt", "r");

    unsigned char nums[10000][14];
    size_t nums_n = 0;

    char s[14][8];
    memset(&s[0][0], 0, 14*8);

    unsigned long digits_1478_in_output = 0;

    while (fscanf(f, "%s %s %s %s %s %s %s %s %s %s | %s %s %s %s\n", s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[8], s[9], s[10], s[11], s[12], s[13]) == 14){
        for (size_t i = 0; i < 14; i++) {
            nums[nums_n][i] = 0;
            for (char *c = &s[i][0]; *c != 0; c++) {
                nums[nums_n][i] |= 1 << (*c - 'a');
            }
            if (i > 9) {
                switch (popcount(nums[nums_n][i])) {
                    case 2:
                    case 3:
                    case 4:
                    case 7:
                        digits_1478_in_output++;
                        break;
                    default:
                        break;
                }
            }
        }
        nums_n++;
    }
    printf("%zu\n", digits_1478_in_output);

    unsigned long sum = 0;
    for (size_t i = 0; i < nums_n; i++) {
        int mapping[128] = {-1};
        unsigned char abfg = 0x7f;
        unsigned char adg = 0x7f;
        unsigned char cf = 0x7f;
        unsigned char bcdf = 0x7f;
        unsigned char acf = 0x7f;

        for (size_t j = 0; j < 10; j++) {
            unsigned char s = nums[i][j];
            switch (popcount(nums[i][j])) {
                case 2:
                    cf &= s;
                    mapping[s] = 1;
                    break;
                case 3:
                    acf &= s;
                    mapping[s] = 7;
                    break;
                case 4:
                    bcdf &= s;
                    mapping[s] = 4;
                    break;
                case 7:
                    mapping[s] = 8;
                    break;
                case 6:
                    abfg &= s;
                    break;
                case 5:
                    adg &= s;
                    break;
                default:
                    printf("what!?");
                    exit(1);
            }
        }

        int a = (abfg) & (adg) & (~cf) & (~bcdf) & (acf);
        int b = abfg & (~adg) & (~cf) & bcdf & (~acf);
        int c = (~abfg) & (~adg) & cf & bcdf & acf;
        int d = (~abfg) & (adg) & (~cf) & (bcdf) & (~acf);
        int e = (~abfg) & (~adg) & (~cf) & (~bcdf) & (~acf) & 0x7f;
        int f = (abfg) & (~adg) & (cf) & (bcdf) & (acf);
        int g = (abfg) & (adg) & (~cf) & (~bcdf) & (~acf);
        assert(popcount(a) == 1);
        assert(popcount(b) == 1);
        assert(popcount(c) == 1);
        assert(popcount(d) == 1);
        assert(popcount(e) == 1);
        assert(popcount(f) == 1);
        assert(popcount(g) == 1);
        /*
            0:    6:      9:         2:      3:    5:      
             aaaa    aaaa    aaaa     aaaa    aaaa    aaaa
            b    c  b    .  b    c   .    c  .    c  b    .
            b    c  b    .  b    c   .    c  .    c  b    .
             ....    dddd    dddd     dddd    dddd    dddd
            e    f  e    f  .    f   e    .  .    f  .    f
            e    f  e    f  .    f   e    .  .    f  .    f
             gggg    gggg    gggg     gggg    gggg    gggg
        */
        mapping[a | b | c | e | f | g] = 0;
        mapping[a | b | d | e | f | g] = 6;
        mapping[a | b | c | d | f | g] = 9;
        mapping[a | c | d | e | g] = 2;
        mapping[a | c | d | f | g] = 3;
        mapping[a | b | d | f | g] = 5;

        unsigned long tens = 1000;
        unsigned long last_sum = sum;

        for (size_t j = 10; j < 14; j++) {
            int n = mapping[nums[i][j]];
            assert(n != -1);
            sum += tens * n;
            tens /= 10;
        }
    }
    printf("%zu\n", sum);
}
