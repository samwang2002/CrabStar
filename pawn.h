#ifndef PAWN_H
#define PAWN_H

#include "move.h"
#include "bitboard.h"

// pawn attacks table [side][square]
extern U64 pawn_attacks[2][64];

// generate pawn attacks
U64 mask_pawn_attacks(const int side, const int square);

// initialize pawn attack table
void init_pawn_attacks();

// generate pawn moves
void generate_pawn_moves(move_list *moves, const int side, const U64 *bitboards, const U64 *occupancies,
                         const int enpassant);

#endif