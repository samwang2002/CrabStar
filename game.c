#include "game.h"
#include "search.h"
#include "board.h"
#include "constants.h"
#include "net.h"
#include <stdio.h>
#include <math.h>
#include <pthread.h>

// returns result of match between player1 (white) and player2 (black)
// small bonus for using fewer nodes
float single_match(const net_weights *player1, const net_weights *player2, const char *start_fen, const int depth,
            const int verbose)
{
    int move_count = 0;
    int w_nodes = 0;
    int b_nodes = 0;
    int winner = 0;

    // initialize position
    board_state board = {};
    parse_fen(&board, start_fen);
    search_state search = {};

    // simulate game
    while (move_count++ < max_moves) {
        // white moves
        int w_move = quick_search(&search, &board, depth, player1);
        make_move(&board, w_move, all_moves);
        w_nodes += search.neg_nodes;

        if (verbose) {
            printf("%d ", move_count);
            print_move(w_move);
            printf(" ");
        }

        if (!has_legal_moves(&board)) {
            if (verbose) printf("\n");
            winner = (square_attacked(&board, ls1b(board.bitboards[k]), white) ? 1 : 0);
            break;
        }

        // black moves
        int b_move = quick_search(&search, &board, depth, player2);
        make_move(&board, b_move, all_moves);
        b_nodes += search.neg_nodes;

        if (verbose) {
            print_move(b_move);
            printf(" %d %d\n", w_nodes, b_nodes);
        }

        if (!has_legal_moves(&board)) {
            winner = (square_attacked(&board, ls1b(board.bitboards[K]), black) ? -1 : 0);
            break;
        }
    }
    // print_board(&board);
    return winner + ((w_nodes < b_nodes) ? node_bonus : -node_bonus);
}

// takes in match_params structure and simulates match between players, writing elo results to array
void *thread_match(void *params)
{
    match_params *args = (match_params *)params;
    float result = single_match(args->player1, args->player2, args->start_fen, args->depth, 0);
    printf("%d vs %d: %0.2f\n", args->player1_num, args->player2_num, result);
    adjust_elos(args->elo1, args->elo2, result);

    return NULL;
    // pthread_exit(NULL);
}

void *foo(void *params)
{
    printf("hello\n");
    return NULL;
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

    // simulate matches
    printf("matchups:\n");
    int n_rounds = 2*n_pairings-1;
    int n_threads = 2*n_pairings*n_rounds;      // n_pairing * n_rounds different matchups, play both colors
    pthread_t tid[n_threads];
    match_params params[n_threads];

    for (int round = 0; round < n_rounds; ++round) {
        for (int i = 0; i < n_pairings; ++i) {
            // first match player1 as white
            int w_idx = 2*n_pairings*round + i;
            params[w_idx].player1 = players[idxs1[i]];
            params[w_idx].player2 = players[idxs2[i]];
            params[w_idx].player1_num = idxs1[i];
            params[w_idx].player2_num = idxs2[i];
            params[w_idx].start_fen = start_position;
            params[w_idx].depth = depth;
            params[w_idx].elo1 = &elos[idxs1[i]];
            params[w_idx].elo2 = &elos[idxs2[i]];
            pthread_create(&tid[w_idx], NULL, thread_match, (void *)&params[w_idx]);

            // second match player1 as black
            int b_idx = w_idx + n_pairings;
            params[b_idx].player1 = players[idxs2[i]];
            params[b_idx].player2 = players[idxs1[i]];
            params[b_idx].player1_num = idxs2[i];
            params[b_idx].player2_num = idxs1[i];
            params[b_idx].start_fen = start_position;
            params[b_idx].depth = depth;
            params[b_idx].elo1 = &elos[idxs2[i]];
            params[b_idx].elo2 = &elos[idxs1[i]];
            pthread_create(&tid[b_idx], NULL, thread_match, (void *)&params[b_idx]);
        }

        // cycle pairings
        for (int i = 1; i < n_pairings; ++i) idxs1[i] = (idxs1[i]>1) ? idxs1[i]-1 : 2*n_pairings-1;
        for (int i = 0; i < n_pairings; ++i) idxs2[i] = (idxs2[i]>1) ? idxs2[i]-1 : 2*n_pairings-1;
    }

    // join all threads
    for (int i = 0; i < n_threads; ++i)
        pthread_join(tid[i], NULL);

    // print new elos
    printf("-------------------\nnew ratings:\n");
    for (int i = 0; i < 2*n_pairings; ++i)
        printf("player %d: %d\n", i, elos[i]);
}

// adjust elo ratings for two players based on result
void adjust_elos(int *elo1, int *elo2, int result)
{
    float expected = 1.0 / (1.0 + pow(10, (*elo1-*elo2)*0.02));
    float actual = result*0.5 + 0.5;
    *elo1 += (int)(elo_k*(actual-expected));
    *elo2 -= (int)(elo_k*(actual-expected));
}
