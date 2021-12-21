#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <assert.h>
#define _GNU_SOURCE 1

static int die = 0;

static int roll_p1() {
    int roll;
    die++;
    roll = die;
    die %= 10;
    return roll;
}

static int move(int p, int rsum) {
    return (p + rsum - 1) % 10 + 1;
}

static void move_p1(int *p) {
    *p = move(*p, roll_p1() + roll_p1() + roll_p1());
}

static void swap(int *n1, int *n2) {
    int temp = *n2;
    *n2 = *n1;
    *n1 = temp;
}

static void play_p2(int p1, int p2, int pp1, int pp2, long *p1win, long *p2win, long universes) {
    if (pp2 >= 21) {
        *p2win += universes;
    } else {
        play_p2(p2, move(p1, 6), pp2, pp1 + move(p1, 6), p2win, p1win, universes*7);
        play_p2(p2, move(p1, 5), pp2, pp1 + move(p1, 5), p2win, p1win, universes*6);
        play_p2(p2, move(p1, 7), pp2, pp1 + move(p1, 7), p2win, p1win, universes*6);
        play_p2(p2, move(p1, 8), pp2, pp1 + move(p1, 8), p2win, p1win, universes*3);
        play_p2(p2, move(p1, 4), pp2, pp1 + move(p1, 4), p2win, p1win, universes*3);
        play_p2(p2, move(p1, 3), pp2, pp1 + move(p1, 3), p2win, p1win, universes);
        play_p2(p2, move(p1, 9), pp2, pp1 + move(p1, 9), p2win, p1win, universes);
    }
}

int main() {
    FILE *f = fopen("input.txt", "r");

    int sp1, sp2;
    assert(fscanf(f, "Player 1 starting position: %d\nPlayer 2 starting position: %d\n", &sp1, &sp2) == 2);
    int player1 = sp1, player2 = sp2;
    int pp1 = 0, pp2 = 0;

    int turn;
    for (turn = 0; pp2 < 1000; turn++) {
        move_p1(&player1);
        pp1 += player1;

        swap(&player1, &player2);
        swap(&pp1, &pp2);
    }

    printf("%d*%d = %d\n", 3*turn, pp1, 3*turn*pp1);

    long p1win = 0, p2win = 0;
    play_p2(sp1, sp2, 0, 0, &p1win, &p2win, 1);

    printf("%ld\n", p1win > p2win ? p1win : p2win);
}
