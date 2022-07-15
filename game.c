#include "game.h"
#include "search.h"
#include "board.h"
#include "constants.h"
#include "net.h"
#include <stdio.h>
#include <math.h>

// returns result of match between player1 (white) and player2 (black)
int match(const net_weights *player1, const net_weights *player2, const int depth, const int verbose)
{
    int move_count = 0;

    while (move_count++ < max_moves) {
        // white moves
        negamax(-infinity, infinity, depth, player1);
        make_move(pv_table[0][0], all_moves);

        if (verbose) {
            printf("%d ", move_count);
            print_move(pv_table[0][0]);
            printf(" ");
        }

        if (!has_legal_moves()) return (square_attacked(ls1b(bitboards[k]), white) ? 1 : 0);

        // black moves
        negamax(-infinity, infinity, depth, player2);
        make_move(pv_table[0][0], all_moves);

        if (verbose) {
            print_move(pv_table[0][0]);
            printf("\n");
        }

        if (!has_legal_moves()) return (square_attacked(ls1b(bitboards[K]), black) ? -1 : 0);
    }
    return 0;
}

// writes array of elo results from round robin tournament
void tournament(const net_weights **players, const int n_pairings, const int depth, int *elos)
{
    memset(elos, 0, 2*n_pairings * sizeof(int));

    // in each round, idxs1[i] faces idxs2[i]
    int idxs1[n_pairings], idxs2[n_pairings];
    for (int i = 0; i < n_pairings; ++i) {
        idxs1[i] = i;
        idxs2[i] = 2*n_pairings - i - 1;
    }

    printf("games:\n");
    // loop through rounds
    for (int round = 0; round < 2*n_pairings-1; ++round) {
        // play matches
        for (int i = 0; i < n_pairings; ++i) {
            parse_fen(start_position);
            int result1 = match(players[idxs1[i]], players[idxs2[i]], depth, 0);
            printf("%d vs %d: %d\n", idxs1[i], idxs2[i], result1);
            adjust_elos(&elos[idxs1[i]], &elos[idxs2[i]], result1);

            parse_fen(start_position);
            int result2 = match(players[idxs2[i]], players[idxs1[i]], depth, 0);
            printf("%d vs %d: %d\n", idxs2[i], idxs1[i], result2);
            adjust_elos(&elos[idxs2[i]], &elos[idxs1[i]], result2);
        }

        // cycle pairings
        for (int i = 1; i < n_pairings; ++i) idxs1[i] = (idxs1[i]>1) ? idxs1[i]-1 : 2*n_pairings-1;
        for (int i = 0; i < n_pairings; ++i) idxs2[i] = (idxs2[i]>1) ? idxs2[i]-1 : 2*n_pairings-1;
    }

    // print new elos
    printf("\nnew ratings:\n");
    for (int i = 0; i < 2*n_pairings; ++i)
        printf("player %d: %d\n", i, elos[i]);
}

// adjust elo ratings for two players based on result
void adjust_elos(int *elo1, int *elo2, int result)
{
    float expected = 1.0 / (1.0 + pow(10, (*elo1-*elo2)*0.02));
    float actual = result*0.5 + 0.5;
    *elo1 += (int)(32*(actual-expected));
    *elo2 -= (int)(32*(actual-expected));
}