#ifndef GAME_H
#define GAME_H

#include "net.h"

#define max_moves 100
#define node_bonus 0.1              // bonus in result for using fewer nodes
#define elo_k 16                    // multiplier to calculate elo scores

// start positions for games
#define open_game "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1"
#define open_sicilian "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1"
#define closed_game "rnbqkbnr/ppp1pppp/8/3p4/3P4/8/PPP1PPPP/RNBQKBNR w KQkq - 0 1"
#define indian_defense "rnbqkb1r/pppppppp/5n2/8/3P4/8/PPP1PPPP/RNBQKBNR w KQkq - 0 1"
#define n_starting_positions 4

static const char *starting_positions[4] = {open_game, open_sicilian, closed_game, indian_defense};

// simulate game between two neural networks, return 1 for player 1 win, -1 for loss, 0 for draw,
// with small bonus for using fewer nodes
float match(const net_weights *player1, const net_weights *player2, const int depth, const int verbose);

// writes array of elo results from round robin tournament
void tournament(net_weights **players, const int n_pairings, const int depth, int *elo_results);

// adjust elo ratings for two players based on result
void adjust_elos(int *elo1, int *elo2, int result);

#endif