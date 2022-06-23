#ifndef PAWN_H
#define PAWN_H

#include "move.h"
#include "bitboard.h"

// pawn attacks table [side][square]
extern U64 pawn_attacks[2][64];

// generate pawn attacks
U64 mask_pawn_attacks(int side, int square);

// initialize pawn attack table
void init_pawn_attacks();

// generate pawn moves
void generate_pawn_moves(move_list *moves, int side, U64 *bitboards, U64 *occupancies, int enpassant);

#endif