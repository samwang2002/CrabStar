#ifndef GAME_H
#define GAME_H

#include "net.h"

#define max_moves 100

// simulate game between two neural networks, return 1 for player 1 win, -1 for loss, 0 for draw
int match(const net_weights *player1, const net_weights *player2, const int depth, const int verbose);

// writes array of elo results from round robin tournament
void tournament(const net_weights **players, const int n_pairings, const int depth, int *elo_results);

// adjust elo ratings for two players based on result
void adjust_elos(int *elo1, int *elo2, int result);

#endif