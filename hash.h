#ifndef HASH_H
#define HASH_H

#include "bitboard.h"


//----------------key tables ------------------

// random piece keys [piece][square]
extern U64 piece_keys[12][64];

// random enpassant keys [square]
extern U64 enpassant_keys[64];

// random castling keys
extern U64 castle_keys[16];

// random side key
extern U64 side_key;

// hash/position key
extern U64 hash_key;

// ----------------functions---------------------

// initialize random hash keys
void init_random_keys();

// generate unique position identifier from scratch
U64 generate_hash_key();
#endif