#ifndef LEAPERS_H
#define LEAPERS_H

#include "bitboard.hpp"

//knight attacks table [square]
static U64 knight_attacks[64];

// generate knight attacks
U64 mask_knight_attacks(int square);

//knight attacks table [square]
static U64 king_attacks[64];

// generate knight attacks
U64 mask_king_attacks(int square);

// pawn attacks table [side][square]
static U64 pawn_attacks[2][64];

// generate pawn attacks
U64 mask_pawn_attacks(int side, int square);

#endif