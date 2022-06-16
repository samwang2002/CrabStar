#ifndef MAGICBITBOARD_H
#define MAGICBITBOARD_H

#include "bitboard.hpp"
#include "rook.hpp"
#include "bishop.hpp"

// bishop and rook
enum { rook, bishop};

// pseudo random number state
static inline unsigned int state = 1804289383;

// generate 32-bit pseudo legal numbers
unsigned int get_random_U32_number();

// generate 64-bit psuedo legal numbers
U64 get_random_U64_number();

// generate magic number candidate
U64 generate_magic_number();

// find appropriate magic number
U64 find_magic_number(int square, int relevant_bits, U64 attack_mask);

// initiliaze magic numbers
void init_magic_numbers();

#endif