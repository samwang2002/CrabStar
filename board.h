#ifndef BOARD_H
#define BOARD_H

#include "bitboard.h"
#include "move.h"
#include <string.h>

// FEN debug positions
#define empty_board "8/8/8/8/8/8/8/8 w - - "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "

/*----------------------------------macros-----------------------------------*/
// preserve board state
#define copy_board()                                                    \
    U64 bitboards_copy[12], occupancies_copy[3];                        \
    int side_copy, enpassant_copy, castle_copy;                         \
    memcpy(bitboards_copy, bitboards, sizeof(bitboards));               \
    memcpy(occupancies_copy, occupancies, sizeof(occupancies));         \
    side_copy = side, enpassant_copy = enpassant, castle_copy = castle; \


// restore board state
#define take_back()                                                     \
    memcpy(bitboards, bitboards_copy, sizeof(bitboards));               \
    memcpy(occupancies, occupancies_copy, sizeof(occupancies));         \
    side = side_copy, enpassant = enpassant_copy, castle = castle_copy; \

/* ------------------------------- functions ------------------------------- */
// print state of board
void print_board();

// initialize all attack tables
void init_attacks();

// initialize position from FEN string
void parse_fen(const char *fen);

// get whether square is currently attacked by given side
int square_attacked(const int square, const int side);

// get whether side has legal moves
int has_legal_moves();

// generate all moves
void generate_moves(move_list *moves);

// make move on chess board
int make_move(const int move, const int move_flag);

// search position for best move and print it to UCI interface
void search_position(const int depth);

// position evaluation
int evaluate();

//negamax alpha beta search
int negamax(int alpha, int beta, int depth);

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

// half move counter
extern int ply;

// best move
extern int best_move;

// negamax alpha beta nodes
extern int neg_nodes;

#endif