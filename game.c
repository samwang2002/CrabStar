#include "game.h"
#include "search.h"
#include "board.h"
#include "constants.h"
#include "net.h"
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>

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
    adjust_elos(args->elo1, args->elo2, result);

    return NULL;
}

// writes array of elo results from round robin tournament
void round_robin(net_weights **players, const int n_players, const int depth, int *elos)
{
    int n_pairings = n_players/2;

    // in each round, idxs1[i] faces idxs2[i]
    int idxs1[n_pairings], idxs2[n_pairings];
    for (int i = 0; i < n_pairings; ++i) {
        idxs1[i] = i;
        idxs2[i] = 2*n_pairings - i - 1;
    }

    // simulate matches
    int n_rounds = 2*n_pairings-1;
    int n_threads = 2*n_pairings;
    pthread_t tid[n_threads];
    match_params params[n_threads];

    for (int round = 0; round < n_rounds; ++round) {
        for (int i = 0; i < n_pairings; ++i) {
            // first match player1 as white
            int w_idx = i;
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

        // join all threads
        for (int i = 0; i < n_threads; ++i)
            pthread_join(tid[i], NULL);

        // cycle pairings
        for (int i = 1; i < n_pairings; ++i) idxs1[i] = (idxs1[i]>1) ? idxs1[i]-1 : 2*n_pairings-1;
        for (int i = 0; i < n_pairings; ++i) idxs2[i] = (idxs2[i]>1) ? idxs2[i]-1 : 2*n_pairings-1;
    }
}

// adjust elo ratings for two players based on result
void adjust_elos(int *elo1, int *elo2, int result)
{
    float expected = 1.0 / (1.0 + pow(10, (*elo1-*elo2)*0.02));
    float actual = result*0.5 + 0.5;
    *elo1 += (int)(elo_k*(actual-expected));
    *elo2 -= (int)(elo_k*(actual-expected));
}

// simulate multiple generations of evolution
void simulate_generations(const int generations, const int n_players, const int depth,
                          const char *seed_path, const int inv_rate, const float std_dev)
{
    net_weights *players[n_players];

    // create seed generation
    players[0] = calloc(1, sizeof(net_weights));
    read_weights(players[0], seed_path);
    for (int i = 1; i < n_players; ++i) {
        players[i] = duplicate_weights(players[0]);
        mutate(players[i], inv_rate/2, 2*std_dev);        // a bit more variety in seed generation
    }

    int elos[n_players];

    // loop through generations
    for (int gen = 0; gen <= generations; ++gen) {
        printf("generation %d\n", gen);

        // simulate tournament
        memset(elos, 0, sizeof(elos));
        round_robin(players, n_players/2, depth, elos);
        for (int i = 0; i < n_players; ++i)
            printf("%d: %d\n", i, elos[i]);

        // if all generations are complete, skip to saving
        if (gen == generations) {
            // find best performing net
            int best_idx = 0;
            for (int i = 1; i < n_players; ++i) if (elos[i] > elos[best_idx]) best_idx = i;

            // save weights
            char dir_path[100];
            sprintf(dir_path, "gen%d", gen);
            printf("%s\nsaving to %s\n", horizontal_line, dir_path);
            save_weights(players[best_idx], dir_path);

            continue;
        }

        // keep 2 top performing nets
        for (int i = 0; i < 2; ++i) {
            int best_idx = i;
            for (int j = i+1; j < n_players; ++j)
                if (elos[j] > elos[best_idx]) best_idx = j;
            
            int num_temp = elos[i];
            elos[i] = elos[best_idx];
            elos[best_idx] = num_temp;

            net_weights *weights_temp = players[i];
            players[i] = players[best_idx];
            players[best_idx] = weights_temp;
        }

        // discard low performing nets
        for (int i = 2; i < n_players; ++i) {
            free(players[i]);
            players[i] = crossover(players[0], players[1], inv_rate, std_dev);
        }

        // save best performing net every 10 generations
        if (gen && gen%10 == 0) {
            char dir_path[100];
            sprintf(dir_path, "gen%d", gen);
            printf("%s\nsaving to %s\n", horizontal_line, dir_path);
            save_weights(players[0], dir_path);
        }

        printf("%s\n", horizontal_line);
    }

    // free all mallocs
    for (int i = 0; i < n_players; ++i)
        free(players[i]);
}