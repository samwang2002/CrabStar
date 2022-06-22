#ifndef QUEEN_HPP
#define QUEEN_HPP

#include "move.hpp"
#include "bitboard.hpp"

// get queen attacks given square and occupancy
U64 get_queen_attacks(int square, U64 occupancy);

// generate queen moves
void generate_queen_moves(move_list *moves, int side, U64 *bitboards, U64 *occupancies);

#endif