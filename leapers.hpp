#ifndef LEAPERS_H
#define LEAPERS_H

#include "bitboard.hpp"

/* ------------------------------- king ------------------------------- */
// king attacks table [square]
extern U64 king_attacks[64];

// generate king attacks
U64 mask_king_attacks(int square);

/* ------------------------------- knight ------------------------------- */
// knight attacks table [square]
extern U64 knight_attacks[64];

// generate knight attacks
U64 mask_knight_attacks(int square);

/* ------------------------------- pawn ------------------------------- */
// pawn attacks table [side][square]
extern U64 pawn_attacks[2][64];

// generate pawn attacks
U64 mask_pawn_attacks(int side, int square);

#endif