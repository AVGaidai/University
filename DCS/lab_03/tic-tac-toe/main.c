#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>


int field[9] = { 0, 0, 0,
                 0, 0, 0,
                 0, 0, 0  };

int PAUSE = 0, START = 0, STOP = 0;

int timeThink = 2;
int timeRestart = 5;
int empty_slots = 9;

void print_field (void)
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (field[i * 3 + j] == 0) {
                printf ("   ");
            } else if (field[i * 3 + j] == 1) {
                printf ("\E[32m X \E[0m");
            } else if (field[i * 3 + j] == 2) {
                printf ("\E[33m O \E[0m");
            }
        }
        printf ("\n");
    }
}


int chek_win (void)
{
    int h = 0, v = 0, d1 = 0, d2 = 0;
    int combo[4];

    combo[2] = field[0];
    combo[3] = field[2];

    for (int i = 0; i < 3; ++i) {
        combo[0] = field[i * 3];
        combo[1] = field[i];

        h = 0;
        v = 0;
        for (int j = 0; j < 3; ++j) {
            if (field[i * 3 + j] == combo[0]) {
                ++h;
                if (h == 3 && combo[0] != 0) return combo[0];
            }
            if (field[j * 3 + i] == combo[1]) {
                ++v;
                if (v == 3 && combo[1] != 0) return combo[1];
            }
            if (i == j && combo[2] == field[i * 3 + j]) {
                ++d1;
                if (d1 == 3 && combo[2] != 0) return combo[2];
            }
            if ((i + j) == 2 && combo[2] == field[i * 3 + j]) {
                ++d2;
                if (d2 == 3 && combo[3] != 0) return combo[3];
            }
        }
    }

    return 0;
}


void *game (void *arg)
{
    int player = 1;
    int pos, res;

    while (!START);

    while (empty_slots && START) {
        while (PAUSE);

        printf ("\E[H\E[J");
        printf ("\n\E[31mTurn p%d!\E[0m\n\n", player);
        print_field ();
        sleep (timeThink);
        
        do {
            pos = rand () % 9;
        } while (field[pos] != 0);
        field[pos] = player;
    
        if ((res = chek_win ()) != 0) {
            printf ("\E[H\E[J");
            printf ("\n\E[34mPlayer %d is WON!\E[0m\n\n", res);
            print_field ();
            break;
        }
        player = (player == 1) ? 2 : 1;
        --empty_slots;
    }
    if (empty_slots == 0) {
        printf ("\E[H\E[J");
        printf ("\n\E[31mDead Heat!\E[0m\n\n");
        print_field ();
    }

    pthread_exit (NULL);
}


void *key_hdr (void *arg)
{
    char ch;
    int val;

    do {
        scanf ("%c", &ch);

        switch (ch) {
        case 'S':
            PAUSE = 1;
            if (START == 0) {
                START = 1;
                PAUSE = 0;
            } else {
                STOP = 1;
                START = 0;
                PAUSE = 0;
                pthread_exit (NULL);
            }
            break;
        case 'T':
            PAUSE = 1;
            printf ("Input timeThink (1 to 10): ");
            scanf ("%d", &val);
            if (val > 0 && val < 11) timeThink = val;
            PAUSE = 0;
            break;
        case 'A':
            PAUSE = 1;
            printf ("Input timeRestart (5 to 30): ");
            scanf ("%d", &val);
            if (val > 4 && val < 31) timeRestart = val;
            PAUSE = 0;
            break;
        default:
            ;
        }
    } while (1);

    pthread_exit (NULL);
}


int main (int argc, char *argv[])
{
    srand (time (NULL));

    pthread_t key;

    pthread_create (&key, NULL, key_hdr, NULL);

    while (!STOP) {
        pthread_t g;

        pthread_create (&g, NULL, game, NULL);

        pthread_join (g, NULL);

        START = 0, PAUSE = 0;
        empty_slots = 9;

        for (int i = 0; i < 9; ++i) field[i] = 0;

        if (!STOP) sleep (timeRestart);
        printf ("\n\E[H\E[J\n");
        printf ("\E[H\E[J");
    }

    pthread_join (key, NULL);

    return 0;
}
