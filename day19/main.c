#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#define _GNU_SOURCE 1

// FIXME: does not work for any part of any problem

typedef struct {
    int x;
    int y;
    int z;
} position_t;

// haha, this is very wrong
position_t orient(position_t p, int o) {
    int r = o >> 3;
    int n = o & 7;

    for (int r0 = 0; r0 < r; r0++) {
        int temp = p.x;
        p.x = p.y;
        p.y = p.z;
        p.z = temp;
    }
    if (n & 1) p.x = -p.x;
    if (n & 2) p.y = -p.y;
    if (n & 4) p.z = -p.z;
}

int comparePos(const void *av, const void *bv) {
    const position_t *a = av;
    const position_t *b = bv;
    if (a->x == b->x) {
        if (a->y == b->y) {
            return b->z - a->z;
        } else return b->y - a->y;
    } else return b->x - a->x;
}

typedef struct {
    size_t n_positions;
    position_t *positions;
    // position_t position;
} scanner_t;


int main() {
    FILE *f = fopen("input.txt", "r");

    size_t n_scanners = 0;
    scanner_t scanners[50];

    int x, y, z;
    char *line = NULL;
    size_t n = 0;
    ssize_t size;

    while (fscanf(f, "--- scanner %d ---\n", &x) == 1) {
        assert(x == n_scanners);
        scanners[n_scanners].positions = NULL;
        scanners[n_scanners].n_positions = 0;
        size_t cap = 0;
        while ((size = getline(&line, &n, f)) > 1) {
            assert(sscanf(line, "%d,%d,%d", &x, &y, &z) == 3);
            if (scanners[n_scanners].n_positions == cap) {
                cap = (cap == 0) ? 4 : (cap << 1);
                scanners[n_scanners].positions = realloc(scanners[n_scanners].positions, cap*sizeof(position_t));
            }
            position_t p;
            p.x = x;
            p.y = y;
            p.z = z;

            scanners[n_scanners].positions[scanners[n_scanners].n_positions++] = p;
        }
        n_scanners++;
    }

    // this was gonna be so annoying to run through
    qsort(scanners[0].positions, scanners[0].n_positions, sizeof(position_t), comparePos);

    for (size_t i = 1; i < n_scanners; i++) {
        int o;
        for (o = 0; o < 24; o++) {
            if (o > 0)
                for (size_t j = 0; j < scanners[i].n_positions; j++) {
                    scanners[i].positions[j] = orient(scanners[i].positions[j], o);
                }
            qsort(scanners[i].positions, scanners[i].n_positions, sizeof(position_t), comparePos);

            // this was maybe a good idea, who knows
            for (int x_off = -100; x_off <= 100; x_off++) {
                for (int y_off = -100; y_off <= 100; y_off++) {
                    
                }
            }
    }
}
