#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <sys/types.h>
#define _GNU_SOURCE 1

struct result {
    long maxy;
    int in_target;
};

struct result check_velocity(unsigned long vx, long vy, long minx, long maxx, long miny, long maxy) {
    long x = 0, y = 0;
    struct result r;
    r.in_target = 1;

    r.maxy = 0;

    while (x <= maxx && y >= miny) {
        if (r.maxy < y) r.maxy = y;
        if (x >= minx && y <= maxy) return r;

        x += vx;
        y += vy;

        if (vx != 0) vx--;

        vy -= 1;
    }
    r.in_target = 0;
    return r;
}

int main() {
    FILE *f = fopen("input.txt", "r");

    long minx, maxx, miny, maxy;

    fscanf(f, "target area: x=%ld..%ld, y=%ld..%ld\n", &minx, &maxx, &miny, &maxy);

    long biggesty = 0;
    unsigned long amount = 0;

    struct result r;
    for (long vy = -1000; vy < 1000; vy++) {
        for (long vx = 0; vx < 10000; vx++) {
            r = check_velocity(vx, vy, minx, maxx, miny, maxy);
            if (r.in_target) {
                if (r.maxy > biggesty) biggesty = r.maxy;
                amount++;
                fprintf(stderr, "%ld %ld -> %ld (%ld)\n", vx, vy, r.maxy, biggesty);
            }
        }
    }
    printf("%ld\n", biggesty);
    printf("%lu\n", amount);
}
