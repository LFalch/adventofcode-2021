#include <stdio.h>
#include <assert.h>

int main() {
    FILE *f = fopen("input.txt", "r");

    unsigned int last_num, next, increases = 0;
    fscanf(f, "%u", &last_num);
    while (fscanf(f, "%u", &next) == 1) {
        if (next > last_num) increases++;
        last_num = next;
    }
    printf("%u\n", increases);
}
