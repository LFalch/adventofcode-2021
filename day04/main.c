#include <stdio.h>
#include <assert.h>
#include <string.h>

int board_won(int board[25]) {
    int won, won2;
    for (int i = 0; i < 5; i++) {
        won = 1;
        won2 = 1;
        for (int j = 0; j < 5; j++) {
            won &= (board[5*j + i] & 1);
            won2 &= (board[5*i + j] & 1);
        }
        if (won || won2) return 1;
    }
    return 0;
}

int main() {
    FILE *f = fopen("input.txt", "r");

    int numbers_drawn[99];
    size_t numbers_drawn_len = 0;
    int drawn, n;
    char c;

    while ((n = fscanf(f, "%d%c", &drawn, &c)) == 2) {
        numbers_drawn[numbers_drawn_len++] = drawn;
        if (c == '\n') {
            break;
        }
    }

    int boards[100][25];
    size_t boards_n;

    char buf[75];

    while (fread(buf, sizeof(buf), 1, f) == 1) {
        for (int i = 0; i < 25; i++) {
            assert(sscanf(&buf[3 * i], "%d", &n) == 1);
            boards[boards_n][i] = n << 1;
        }
        boards_n++;
        assert(getc(f) == '\n');
    }

    int winners[boards_n];
    memset(winners, 0, sizeof(int) * boards_n);

    for (int i = 0; i < numbers_drawn_len; i++) {
        drawn = numbers_drawn[i];

        for (int j = 0; j < boards_n; j++) {
            if (winners[j]) continue;
            for (int k = 0; k < 25; k++) {
                if (boards[j][k] == (drawn << 1)) {
                    boards[j][k] |= 1;
                }
            }

            if (board_won(boards[j])) {
                int sum = 0;
                for (int k = 0; k < 25; k++) {
                    if (!(boards[j][k] & 1)) {
                        sum += boards[j][k] >> 1;
                    }
                }
                printf("%d * %d\n", drawn, sum);
                printf("%d\n", drawn * sum);
                winners[j] = 1;
            }
        }
    }
    
    for (int i = 0; i < boards_n; i++) {
        if (winners[i]) return 0;
    }

    printf("No one won! :(\n\n");

    for (int i = 0; i < boards_n; i++) {
        printf("%2d %2d %2d %2d %2d\n", boards[i][0]&1, boards[i][1]&1, boards[i][2]&1, boards[i][3]&1, boards[i][4]&1);
        printf("%2d %2d %2d %2d %2d\n", boards[i][1 * 5 + 0]&1, boards[i][1 * 5 + 1]&1, boards[i][1 * 5 + 2]&1, boards[i][1 * 5 + 3]&1, boards[i][1 * 5 + 4]&1);
        printf("%2d %2d %2d %2d %2d\n", boards[i][2 * 5 + 0]&1, boards[i][2 * 5 + 1]&1, boards[i][2 * 5 + 2]&1, boards[i][2 * 5 + 3]&1, boards[i][2 * 5 + 4]&1);
        printf("%2d %2d %2d %2d %2d\n", boards[i][3 * 5 + 0]&1, boards[i][3 * 5 + 1]&1, boards[i][3 * 5 + 2]&1, boards[i][3 * 5 + 3]&1, boards[i][3 * 5 + 4]&1);
        printf("%2d %2d %2d %2d %2d\n", boards[i][4 * 5 + 0]&1, boards[i][4 * 5 + 1]&1, boards[i][4 * 5 + 2]&1, boards[i][4 * 5 + 3]&1, boards[i][4 * 5 + 4]&1);
        printf("\n");
    }

    return 1;
}
