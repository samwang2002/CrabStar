#ifndef BOARD_H
#define BOARD_H

#include "bitboard.hpp"
#include "move.hpp"
#include <map>

// FEN debug positions
#define empty_board "8/8/8/8/8/8/8/8 w - - "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "

/* ------------------------------- functions ------------------------------- */
// print state of board
void print_board();

// initialize attack tables for leapers
void init_leapers_attacks();

// initialize all attacks
void init_all();

// initialize position from FEN string
void parse_fen(const char *fen);

// get whether square is currently attacked by given side
int square_attacked(int square, int side);

// generate all moves
void generate_moves(move_list *moves, int side);

// generate pawn moves
void generate_pawn_moves(move_list *moves, int side);

/* ------------------------------- constants ------------------------------- */
// sides to move (colors)
enum {white, black, both};

// bishop and rook
enum {rook, bishop};

// ASCII pieces
static const char ascii_pieces[13] = "PNBRQKpnbrqk";

// unicode pieces
static const char *unicode_pieces[12] = {"♙", "♘", "♗", "♖", "♕", "♔", "♟", "♞", "♝", "♜", "♛", "♚"};

// convert ASCII character pieces to encoded constants
const std::map<char, int> char_pieces = {
    {'P', P}, {'N', N}, {'B', B}, {'R', R}, {'Q', Q}, {'K', K},
    {'p', p}, {'n', n}, {'b', b}, {'r', r}, {'q', q}, {'k', k}
};

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
extern U64 bitboards[12];

// occupancy bitboards
extern U64 occupancies[3];

// side to move
extern int side;

// enpassant square
extern int enpassant;

// castling rights
extern int castle;

#endif