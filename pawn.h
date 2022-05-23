#ifndef PAWN_H
#define PAWN_H

#include "bitboard.h"

// generate pawn attacks
U64 mask_pawn_attacks(int side, int square);

void init_leapers_attacks();

#endif