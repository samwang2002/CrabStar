#ifndef LEAPERS_H
#define LEAPERS_H

#include "bitboard.hpp"

/* ------------------------------- king ------------------------------- */
// king attacks table [square]
extern U64 king_attacks[64];

// generate king attacks
U64 mask_king_attacks(int square);

#endif