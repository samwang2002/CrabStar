#ifndef PAWN_H
#define PAWN_H

#include "bitboard.h"

// pawn attacks table [side][square]
static U64 pawn_attacks[2][64];

// generate pawn attacks
U64 mask_pawn_attacks(int side, int square);

#endif