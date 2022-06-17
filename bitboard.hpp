#ifndef BITBOARD_H
#define BITBOARD_H


//define bitboard data type
#define U64 unsigned long long

// set/get/pop macros
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))
#define count_bits(bitboard) __builtin_popcountll(bitboard)

// get index of least significant bit
static inline int get_ls1b_index(U64 bitboard)
{
    if (bitboard) return count_bits((bitboard & -bitboard)-1);
    return -1;
}

// sides to move (colors)
enum {white, black, both};

// bishop and rook
enum { rook, bishop};

// encode pieces
enum { P, N, B, R, Q, K, p, n, b, r, q, k };

//ASCII pieces
const char ascii_pieces[13] = "PNBRQKpnbrqk";

// convert ASCII character pieces to encoded constants
// note: the array is in the exact order as C++ does not support designated initializers
const int char_pieces[] = {P, N, B, R, Q, K, P, n, b, r, q, k};

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

static const char* square_to_coordinates[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};

/*
    bin  dec
    
   0001    1  white king can castle to the king side
   0010    2  white king can castle to the queen side
   0100    4  black king can castle to the king side
   1000    8  black king can castle to the queen side
   examples
   1111       both sides an castle both directions
   1001       black king => queen side
              white king => king side
*/
//castling rights binary encoding
enum { wk = 1, wq = 2, bk = 4, bq = 8 };



/**********************\
    not A file
 
  8  0 1 1 1 1 1 1 1
  7  0 1 1 1 1 1 1 1
  6  0 1 1 1 1 1 1 1
  5  0 1 1 1 1 1 1 1 
  4  0 1 1 1 1 1 1 1 
  3  0 1 1 1 1 1 1 1
  2  0 1 1 1 1 1 1 1
  1  0 1 1 1 1 1 1 1

     a b c d e f g h


    not H file
  8  1 1 1 1 1 1 1 0
  7  1 1 1 1 1 1 1 0
  6  1 1 1 1 1 1 1 0
  5  1 1 1 1 1 1 1 0 
  4  1 1 1 1 1 1 1 0 
  3  1 1 1 1 1 1 1 0
  2  1 1 1 1 1 1 1 0
  1  1 1 1 1 1 1 1 0

     a b c d e f g h

    not HG file
  8  1 1 1 1 1 1 0 0
  7  1 1 1 1 1 1 0 0
  6  1 1 1 1 1 1 0 0
  5  1 1 1 1 1 1 0 0 
  4  1 1 1 1 1 1 0 0 
  3  1 1 1 1 1 1 0 0
  2  1 1 1 1 1 1 0 0
  1  1 1 1 1 1 1 0 0

     a b c d e f g h

    not AB file
  8  0 0 1 1 1 1 1 1
  7  0 0 1 1 1 1 1 1 
  6  0 0 1 1 1 1 1 1
  5  0 0 1 1 1 1 1 1 
  4  0 0 1 1 1 1 1 1 
  3  0 0 1 1 1 1 1 1
  2  0 0 1 1 1 1 1 1
  1  0 0 1 1 1 1 1 1

     a b c d e f g h
**********************/

// file constants
const U64 not_a_file = 18374403900871474942ULL;
const U64 not_h_file = 9187201950435737471ULL;
const U64 not_hg_file = 4557430888798830399ULL;
const U64 not_ab_file = 18229723555195321596ULL;


// bitboard functions
void print_bitboard(U64 bitboard);

U64 set_occupancy(int index, int masked_bits, U64 attack_mask);

// bitboards
static U64 bitboards[12];

// occupancy bitboards
static U64 occupancies[3];

// side to move
static int side = -1;

// enpassant square
static int enpassant = no_sq;

// castling rights
static int castle;

#endif