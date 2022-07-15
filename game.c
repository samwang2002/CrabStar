#include "game.h"
#include "search.h"
#include "board.h"
#include "constants.h"
#include "net.h"
#include <stdio.h>
#include <math.h>

// returns result of match between player1 (white) and player2 (black)
// small bonus for using fewer nodes
float match(const net_weights *player1, const net_weights *player2, const int depth, const int verbose)
{
    int move_count = 0;
    int w_nodes = 0;
    int b_nodes = 0;
    int winner = 0;

    while (move_count++ < max_moves) {
        // white moves
        int w_move = quick_search(depth, player1);
        make_move(w_move, all_moves);
        w_nodes += neg_nodes;

        if (verbose) {
            printf("%d ", move_count);
            print_move(w_move);
            printf(" ");
        }

        if (!has_legal_moves()) {
            if (verbose) printf("\n");
            winner = (square_attacked(ls1b(bitboards[k]), white) ? 1 : 0);
            break;
        }

        // black moves
        int b_move = quick_search(depth, player2);
        make_move(b_move, all_moves);
        b_nodes += neg_nodes;

        if (verbose) {
            print_move(b_move);
            printf(" %d %d\n", w_nodes, b_nodes);
        }

        if (!has_legal_moves()) {
            winner = (square_attacked(ls1b(bitboards[K]), black) ? -1 : 0);
            break;
        }
    }
    return winner + ((w_nodes < b_nodes) ? node_bonus : -node_bonus);
}

// writes array of elo results from round robin tournament
void tournament(net_weights **players, const int n_pairings, const int depth, int *elos)
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
            float result1 = match(players[idxs1[i]], players[idxs2[i]], depth, 0);
            printf("%d vs %d: %f\n", idxs1[i], idxs2[i], result1);
            adjust_elos(&elos[idxs1[i]], &elos[idxs2[i]], result1);

            parse_fen(start_position);
            float result2 = match(players[idxs2[i]], players[idxs1[i]], depth, 0);
            printf("%d vs %d: %f\n", idxs2[i], idxs1[i], result2);
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