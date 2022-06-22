#ifndef KING_HPP
#define KING_HPP

#include "move.hpp"
#include "bitboard.hpp"

// king attacks table [square]
extern U64 king_attacks[64];

// generate king attacks
U64 mask_king_attacks(int square);

// initialize king attack table
void init_king_attacks();

// generate castling moves
void generate_castling_moves(move_list *moves, int side, U64 *bitboards, U64 *occupancies, int castle,
                             int (*attacked)(int, int));

// generate non-castling king moves
void generate_king_moves(move_list *moves, int side, U64 *bitboards, U64 *occupancies);

#endif