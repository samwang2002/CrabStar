#ifndef HASH_H
#define HASH_H

#include "bitboard.h"


//----------------key tables ------------------

// random piece keys [piece][square]
static U64 piece_keys[12][64];

// random enpassant keys [square]
static U64 enpassant_keys[64];

// random castling keys
static U64 castle_keys[16];

// random side key
static U64 side_key;

// hash/position key
static U64 hash_key;

// ----------------functions---------------------

// initialize random hash keys
void init_random_keys();

// generate unique position identifier from scratch
U64 generate_hash_key();
#endif