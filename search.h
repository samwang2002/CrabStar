#ifndef SEARCH_H
#define SEARCH_H

#include "bitboard.h"
#include "move.h"

#define max_ply 64

/* ------------------------------- functions ------------------------------- */
// score move - determines priority of move
int score_move(const int move);

// sort moves in descending order by score
void sort_moves(move_list *moves);

// negamax alpha beta search
int negamax(const int alpha, const int beta, int depth, const float net_weight);

// quiescence search: similar to negamax but only examine captures
int quiescence(const int alpha, const int beta, const float net_weight);

// search position for best move and print it to UCI interface
void search_position(const int depth, const float net_weight);

// enable PV mopve scoring
void enable_pv_scoring(move_list *moves);

/* ------------------------------- variables ------------------------------- */
// best move
extern int best_move;

// negamax alpha beta nodes
extern int neg_nodes;

// killer moves [id][ply]
// can use different number of killer moves but 2 is a good choice
// see: https://www.chessprogramming.org/Killer_Heuristic
extern int killer_moves[2][max_ply];

// history moves [piece][square]
// prioritizes moves that cause more cutoffs in search tree
// see: https://www.chessprogramming.org/History_Heuristic
extern int history_moves[12][64];

// PV (principal variation) length [ply]
extern int pv_length[max_ply];

// PV table
// see: https://www.reddit.com/r/chessprogramming/comments/m2m048/how_does_a_triangular_pvtable_work/
extern int pv_table[max_ply][max_ply];
/*
    diagonal structure:
    m1 m2 m3  ... <- principal variation
    0  m2 m3  ...
    0  0  m3  ...
    .  .  .
*/

// follow PV flag
extern int follow_pv;

// score PV PV flag
extern int score_pv;

//late move reduction variables
extern int full_depth_moves;
extern int reduction_limit;

#endif