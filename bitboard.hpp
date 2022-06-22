#ifndef BITBOARD_HPP
#define BITBOARD_HPP


// define bitboard data type
#define U64 unsigned long long

/* ------------------------------- macros ------------------------------- */
// returns bitboard with square-th bit set as 1
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))

// returns value of square-th bit of bitboard
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))

// returns bitboard with square-th bit set as 0
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

// returns number of set bits in bitboard
#define count_bits(bitboard) __builtin_popcountll(bitboard)

/* ------------------------------- function declarations ------------------------------- */
// get index of least significant bit
int get_ls1b_index(U64 bitboard);

// print bitboard as grid of 0s and 1s
void print_bitboard(U64 bitboard);

// initializes bitboard using index to describe which bits in attack mask are set
U64 set_occupancy(int index, int masked_bits, U64 attack_mask);

/* ------------------------------- constants ------------------------------- */
// board squares
enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, no_sq
};

static const char *square_to_coordinates[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};

// file constants
static const U64 not_a_file = 18374403900871474942ULL;
static const U64 not_h_file = 9187201950435737471ULL;
static const U64 not_hg_file = 4557430888798830399ULL;
static const U64 not_ab_file = 18229723555195321596ULL;

/**********************\
  ex: not A file
 
  8  0 1 1 1 1 1 1 1
  7  0 1 1 1 1 1 1 1
  6  0 1 1 1 1 1 1 1
  5  0 1 1 1 1 1 1 1 
  4  0 1 1 1 1 1 1 1 
  3  0 1 1 1 1 1 1 1
  2  0 1 1 1 1 1 1 1
  1  0 1 1 1 1 1 1 1

     a b c d e f g h
**********************/

#endif