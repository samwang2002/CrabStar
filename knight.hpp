#ifndef KNIGHT_HPP
#define KNIGHT_HPP

#include "move.hpp"
#include "bitboard.hpp"

// knight attacks table [square]
static U64 knight_attacks[64];

// generate knight attacks
U64 mask_knight_attacks(int square);

// initialize knight attack table
void init_knight_attacks();

// generate knight moves
void generate_knight_moves(move_list *moves, int side, U64 *bitboards, U64 *occupancies);

#endif