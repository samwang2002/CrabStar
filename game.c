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
    print_board(&board);
    return winner + ((w_nodes < b_nodes) ? node_bonus : -node_bonus);
}

// takes in match_params structure and simulates match between players, writing elo results to array
void *thread_match(void *params)
{
    match_params *args = (match_params *)params; 
    float result = single_match(args->player1, args->player2, args->start_fen, args->depth, 1);
    printf("%d vs %d: %0.2f\n", args->player1_num, args->player2_num, result);
    adjust_elos(args->elo1, args->elo2, result);

    pthread_exit(NULL);
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

    // pthread_t tid2;
    // match_params params2 = (match_params){ .player1 = players[0], .player2 = players[1],
    //                                     .player1_num = 0, .player2_num = 1,
    //                                     .start_fen = start_position, .depth = depth,
    //                                     .elo1 = &elos[0], .elo2 = &elos[1] };
    // pthread_create(&tid2, NULL, thread_match, (void *)&params2);
    // pthread_join(tid2, NULL);

    // loop through rounds
    printf("matchups:\n");
    for (int round = 0; round < 2*n_pairings-1; ++round) {
        // play matches
        pthread_t tid[n_pairings];
        for (int i = 0; i < n_pairings; ++i) {
            // loop through starting positions
            // for (int j = 0; j < n_starting_positions; ++j) {
                match_params params = (match_params){ .player1 = players[idxs1[i]], .player2 = players[idxs2[i]],
                                        .player1_num = idxs1[i], .player2_num = idxs2[i],
                                        .start_fen = start_position, .depth = depth,
                                        .elo1 = &elos[idxs1[i]], .elo2 = &elos[idxs2[i]] };
                pthread_create(&tid[i], NULL, thread_match, (void *)&params);
            // }
        }

        // join all threads
        for (int i = 0; i < n_pairings; ++i)
            pthread_join(tid[i], NULL);
        
        printf("--------------------\n");

        // cycle pairings
        for (int i = 1; i < n_pairings; ++i) idxs1[i] = (idxs1[i]>1) ? idxs1[i]-1 : 2*n_pairings-1;
        for (int i = 0; i < n_pairings; ++i) idxs2[i] = (idxs2[i]>1) ? idxs2[i]-1 : 2*n_pairings-1;
    }

    // print new elos
    printf("new ratings:\n");
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
