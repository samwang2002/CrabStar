#ifndef KING_H
#define KING_H

#include "move.h"
#include "bitboard.h"

// king attacks table [square]
extern U64 king_attacks[64];

// generate king attacks
U64 mask_king_attacks(const int square);

// initialize king attack table
void init_king_attacks();

// // generate castling moves
// void generate_castling_moves(move_list *moves, const int side, const U64 *bitboards, const U64 *occupancies,
//                              const int castle, int (* const attacked)(const int, const int));

// generate non-castling king moves
void generate_king_moves(move_list *moves, const int side, const U64 *bitboards, const U64 *occupancies);

#endif