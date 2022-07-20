#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <pthread.h>

#include "board.h"
#include "bitboard.h"
#include "move.h"
#include "constants.h"
#include "net.h"
#include "search.h"
#include "game.h"
#include "random.h"

#include "pawn.h"
#include "knight.h"
#include "king.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"

int main()
{
    init_all();
    srand(time(NULL));

    clock_t start, end;
    start = clock();

    // create array of players
    int n_players = 8;
    net_weights *players[n_players];

    players[0] = calloc(1, sizeof(net_weights));
    read_weights(players[0], "seed1");
    for (int i = 1; i < n_players; ++i) {
        players[i] = duplicate_weights(players[0]);
        mutate(players[i], 5, 0.2);
    }

    // have players face off
   int results[n_players];
   tournament(players, n_players/2, 3, results);

    // free player array
    for (int i = 0; i < n_players; ++i)
        free(players[i]);

    end = clock();
    printf("-------------------\ntime taken: %0.2fs\n", (double)(end-start)/CLOCKS_PER_SEC);

    return 0;
}
