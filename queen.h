#ifndef QUEEN_H
#define QUEEN_H

#include "move.h"
#include "bitboard.h"

// get queen attacks given square and occupancy
U64 get_queen_attacks(const int square, const U64 occupancy);

// generate queen moves
void generate_queen_moves(move_list *moves, const int side, const U64 *bitboards, const U64 *occupancies);

#endif