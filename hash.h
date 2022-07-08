#ifndef HASH_H
#define HASH_H

#include "bitboard.h"

//---------------transposition table------------

// hash table size
#define hash_size 0x400000

// tranposition table hash flags
#define hash_flag_exact 0
#define hash_flag_alpha 1
#define hash_flag_beta 2

//transposition table data structure
typedef struct {
    U64 hash_key; // unique chess position identifier
    int depth; // current search depth
    int flag; // flag the type of node
    int score; // score (alpha/beta/PV)
} tt;

extern tt hash_table[hash_size];

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

void clear_hash_table();
#endif