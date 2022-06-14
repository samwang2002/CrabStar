#ifndef KNIGHT_H
#define KNIGHT_H

#include "bitboard.hpp"

//knight attacks table [square]
static U64 knight_attacks[64];

// generate knight attacks
U64 mask_knight_attacks(int square);

#endif