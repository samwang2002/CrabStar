#ifndef KNIGHT_H
#define KNIGHT_H

#include "move.h"
#include "bitboard.h"

// knight attacks table [square]
extern U64 knight_attacks[64];

// generate knight attacks
U64 mask_knight_attacks(int square);

// initialize knight attack table
void init_knight_attacks();

// generate knight moves
void generate_knight_moves(move_list *moves, int side, U64 *bitboards, U64 *occupancies);

#endif