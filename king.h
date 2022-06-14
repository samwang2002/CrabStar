#ifndef KING_H
#define KING_H

#include "bitboard.h"

//knight attacks table [square]
static U64 king_attacks[64];

// generate knight attacks
U64 mask_king_attacks(int square);

#endif