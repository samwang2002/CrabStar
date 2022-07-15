#ifndef GAME_H
#define GAME_H

#include "net.h"

#define max_moves 100

// simulate game between two neural networks, return 1 for player 1 win, -1 for loss, 0 for draw
int match(const net_weights *player1, const net_weights *player2, const int depth, const int verbose);

#endif