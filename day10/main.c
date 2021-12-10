#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#define _GNU_SOURCE 1

int comp(const void *a, const void *b) {
    unsigned long x = *(unsigned long *) a;
    unsigned long y = *(unsigned long *) b;

    return (x > y ? 1 : (x == y ? 0 : -1));
}

int main() {
    FILE *f = fopen("input.txt", "r");

    char completion[200];
    size_t completion_length = 0;

    unsigned long points = 0;
    size_t points2_n = 0;
    unsigned long points2[500];

    char *line = NULL;
    size_t n = 0;
    ssize_t len;

    while ((len = getline(&line, &n, f)) > 0) {
        completion_length = 0;
        int invalid = 0;
        for (size_t i = 0; i < len && !invalid; i++) {
            switch (line[i]) {
                case '(':
                    completion[completion_length++] = ')';
                    break;
                case '[':
                    completion[completion_length++] = ']';
                    break;
                case '{':
                    completion[completion_length++] = '}';
                    break;
                case '<':
                    completion[completion_length++] = '>';
                    break;
                case ')':
                    if (completion[--completion_length] != line[i]) {
                        points += 3;
                        invalid = 1;
                    }
                    break;
                case ']':
                    if (completion[--completion_length] != line[i]) {
                        points += 57;
                        invalid = 1;
                    }
                    break;
                case '}':
                    if (completion[--completion_length] != line[i]) {
                        points += 1197;
                        invalid = 1;
                    }
                    break;
                case '>':
                    if (completion[--completion_length] != line[i]) {
                        points += 25137;
                        invalid = 1;
                    }
                    break;
            }
        }

        if (!invalid) {
            points2[points2_n] = 0;

            for (size_t i = completion_length-1; i < completion_length; i--) {
                points2[points2_n] *= 5;
                switch (completion[i]) {
                case ')':
                    points2[points2_n] += 1;
                    break;
                case ']':
                    points2[points2_n] += 2;
                    break;
                case '}':
                    points2[points2_n] += 3;
                    break;
                case '>':
                    points2[points2_n] += 4;
                    break;
                }
            }
            points2_n++;
        }
    }
    printf("%lu\n", points);
    qsort(points2, points2_n, sizeof(unsigned long), comp);
    printf("%lu\n", points2[points2_n/2]);
}
