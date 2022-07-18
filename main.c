#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

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

    // net_weights player1, player2;
    // read_weights(&player1, "seed1");
    // player2 = player1;
    // mutate(&player2, 5, 0.2);
    // single_match(&player1, &player2, start_position, 3, 1);

    clock_t start, end;
    start = clock();

    // // create array of players
    // int n_players = 4;
    // net_weights **players = malloc(n_players * sizeof(net_weights *));
    // int results[n_players];

    // players[0] = malloc(sizeof(net_weights));
    // read_weights(players[0], "seed1");

    // for (int i = 1; i < n_players; ++i) {
    //     players[i] = malloc(sizeof(net_weights));
    //     copy_weights(players[i], players[0]);
    //     mutate(players[i], 5, 0.2);
    // }

    // create array of players
    int n_players = 4;
    net_weights *players[n_players];

    players[0] = malloc(sizeof(net_weights));
    read_weights(players[0], "seed1");
    for (int i = 1; i < n_players; ++i) {
        players[i] = duplicate_weights(players[0]);
        mutate(players[i], 5, 0.2);
    }

    // single_match(players[0], players[1], start_position, 3, 1);
    // single_match(players[1], players[2], start_position, 3, 1);


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