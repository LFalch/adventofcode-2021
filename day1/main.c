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
    printf("Indidviual increases: %u\n", increases);
    fclose(f);

    f = fopen("input.txt", "r");
    increases = 0;
    unsigned int last_last_last_num, last_last_num;
    fscanf(f, "%u", &last_last_last_num);
    fscanf(f, "%u", &last_last_num);
    fscanf(f, "%u", &last_num);
    while (fscanf(f, "%u", &next) == 1) {
        unsigned last_sum = last_last_last_num + last_last_num + last_num;
        unsigned next_sum = last_last_num + last_num + next;
        if (next_sum > last_sum) increases++;
        last_last_last_num = last_last_num;
        last_last_num = last_num;
        last_num = next;
    }
    printf("Window3 increases: %u\n", increases);
    fclose(f);
}
