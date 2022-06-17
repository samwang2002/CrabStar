#ifndef BOARD_H
#define BOARD_H

#include "bitboard.hpp"

/* ------------------------------- functions ------------------------------- */
// print state of board
void print_board();

// initialize attack tables for leapers
void init_leapers_attacks();

// initialize all attacks
void init_all();

/* ------------------------------- constants ------------------------------- */
// sides to move (colors)
enum {white, black, both};

// bishop and rook
enum {rook, bishop};

// encode pieces
enum {P, N, B, R, Q, K, p, n, b, r, q, k};

//ASCII pieces
const char ascii_pieces[13] = "PNBRQKpnbrqk";

// convert ASCII character pieces to encoded constants
// note: the array is in the exact order as C++ does not support designated initializers
const int char_pieces[] = {P, N, B, R, Q, K, p, n, b, r, q, k};

//castling rights binary encoding
enum { wk = 1, wq = 2, bk = 4, bq = 8 };
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


/* ------------------------------- variables ------------------------------- */
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