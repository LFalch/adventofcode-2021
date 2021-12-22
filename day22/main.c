#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <tgmath.h>
#include <sys/types.h>
#define _GNU_SOURCE 1

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define pos(x) ((size_t) ((x) <= 0 ? 0 : (x)))

static void part1() {
    FILE *f = fopen("input.txt", "r");

    int cubes[101][101][101] = {0};
    char onoff[4];
    int xmin, xmax, ymin, ymax, zmin, zmax;

    while (fscanf(f, "%s x=%d..%d,y=%d..%d,z=%d..%d\n", onoff, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax) == 7) {
        int on = strcmp("on", onoff) == 0;
        if (!on) assert(strcmp("off", onoff) == 0);
        if (xmin >= -50 && xmax <= 50 && ymin >= -50 && ymax <= 50 && zmin >= -50 && zmax <= 50) {
            for (size_t i = xmin+50L; i <= xmax+50L; i++) {
                for (size_t j = ymin + 50L; j <= ymax + 50L; j++) {
                    for (size_t k = zmin + 50L; k <= zmax + 50L; k++) {
                        cubes[i][j][k] = on;
                    }
                }
            }
        }
    }

    size_t count = 0;

    for (size_t i = -50 + 50L; i <= 50L + 50L; i++) {
        for (size_t j = -50 + 50L; j <= 50L + 50L; j++) {
            for (size_t k = -50 + 50L; k <= 50L + 50L; k++) {
                if (cubes[i][j][k]) count++;
            }
        }
    }

    printf("%zu\n", count);
}

typedef struct {
    int xmin, xmax, ymin, ymax, zmin, zmax;
} cube_t;

typedef struct {
    cube_t cube;
    char on;
} line_t;

static cube_t cb(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax) {
    cube_t ret;
    ret.xmin = xmin;
    ret.xmax = xmax;
    ret.ymin = ymin;
    ret.ymax = ymax;
    ret.zmin = zmin;
    ret.zmax = zmax;
    return ret;
}

static cube_t intersection(const cube_t *a, const cube_t *b) {
    cube_t ret;
    ret.xmin = max(a->xmin, b->xmin);
    ret.xmax = min(a->xmax, b->xmax);
    ret.ymin = max(a->ymin, b->ymin);
    ret.ymax = min(a->ymax, b->ymax);
    ret.zmin = max(a->zmin, b->zmin);
    ret.zmax = min(a->zmax, b->zmax);
    return ret;
}

static size_t measure(const cube_t *c) {
    return pos(c->xmax - c->xmin) * pos(c->ymax - c->ymin) * pos(c->zmax - c->zmin);
}

static cube_t djcubes[6];

static size_t disjoint_cubes(const cube_t *with, const cube_t *without) {
    const cube_t wo = intersection(with, without);

    if (measure(&wo) == 0) {
        djcubes[0] = *with;
        return 1;
    }

    cube_t c = *with;

    size_t n = 0;
    djcubes[n] = c;
    djcubes[n].xmax = wo.xmin;
    if (measure(&djcubes[n]) > 0) n++;
    djcubes[n] = c;
    djcubes[n].xmin = wo.xmax;
    if (measure(&djcubes[n]) > 0) n++;

    c.xmin = wo.xmin;
    c.xmax = wo.xmax;

    if (measure(&c) == 0) return n;

    djcubes[n] = c;
    djcubes[n].ymax = wo.ymin;
    if (measure(&djcubes[n]) > 0) n++;
    djcubes[n] = c;
    djcubes[n].ymin = wo.ymax;
    if (measure(&djcubes[n]) > 0) n++;

    c.ymin = wo.ymin;
    c.ymax = wo.ymax;

    if (measure(&c) == 0) return n;

    djcubes[n] = c;
    djcubes[n].zmax = wo.zmin;
    if (measure(&djcubes[n]) > 0) n++;
    djcubes[n] = c;
    djcubes[n].zmin = wo.zmax;
    if (measure(&djcubes[n]) > 0) n++;

    return n;
}

static void part2() {
    FILE *f = fopen("input.txt", "r");

    line_t cubes[500];
    size_t cubes_n = 0;
    char onoff[4];

    while (fscanf(f, "%s x=%d..%d,y=%d..%d,z=%d..%d\n", onoff, &cubes[cubes_n].cube.xmin, &cubes[cubes_n].cube.xmax, &cubes[cubes_n].cube.ymin, &cubes[cubes_n].cube.ymax, &cubes[cubes_n].cube.zmin, &cubes[cubes_n].cube.zmax) == 7) {
        int on = strcmp("on", onoff) == 0;
        if (!on) assert(strcmp("off", onoff) == 0);
        cubes[cubes_n].on = on;
        cubes[cubes_n].cube.xmax++;
        cubes[cubes_n].cube.ymax++;
        cubes[cubes_n].cube.zmax++;

        ++cubes_n;
    }

    size_t count = 0;

    for (size_t i = cubes_n-1; 0 <= i && i < cubes_n; i--) {
        if (cubes[i].on) {
            cube_t *cubes_to_add = malloc(sizeof(cube_t));
            cube_t *cubes_to_add_new = NULL;
            size_t add_n = 1, new_add_n;

            cubes_to_add[0] = cubes[i].cube;

            for (size_t j = i + 1; j < cubes_n; j++) {
                size_t cap = max(add_n, 1);
                cubes_to_add_new = malloc(cap * sizeof(cube_t));
                new_add_n = 0;

                for (size_t k = 0; k < add_n; k++) {
                    size_t djcubes_n = disjoint_cubes(&cubes_to_add[k], &cubes[j].cube);
                    for (size_t ii = 0; ii < djcubes_n; ii++) {
                        cubes_to_add_new[new_add_n] = djcubes[ii];
                        new_add_n++;
                        if (new_add_n == cap) {
                            cap *= 2;
                            cubes_to_add_new = realloc(cubes_to_add_new, cap * sizeof(cube_t));
                        }
                    }
                }
                add_n = new_add_n;
                free(cubes_to_add);
                cubes_to_add = cubes_to_add_new;
            }

            for (size_t j = 0; j < add_n; j++) {
                count += measure(&cubes_to_add[j]);
            }
            free(cubes_to_add);
        }
    }

    printf("%zu\n", count);
}

int main() {
    part1();
    part2();
}
