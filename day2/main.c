#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *f = fopen("input.txt", "r");
    int d = 0, hp = 0, nd = 0, in;
    char *s;
    size_t len;
    while (getline(&s, &len, f) > 0) {
        int i;
        for (i = 0; i < len; i++) {
            if (s[i] == ' ') {
                s[i] = '\0';
                break;
            }
        }
        i++;
        sscanf(&s[i], "%d", &in);

        if (strcmp(s, "forward") == 0) {
            hp += in;
            nd += d * in;
        }
        if (strcmp(s, "up") == 0) d -= in;
        if (strcmp(s, "down") == 0) d += in;
    }
    printf("%d\n", d*hp);
    printf("%d\n", nd*hp);
}
