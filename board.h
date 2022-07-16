#ifndef BOARD_H
#define BOARD_H

#include "bitboard.h"
#include "move.h"
#include "constants.h"
#include <string.h>

// board state structure
typedef struct {
    // positional data
    U64 bitboards[12];
    U64 occupancies[3];
    int side;
    int enpassant;
    int castle;
} board_state;

// FEN debug positions
#define empty_board "8/8/8/8/8/8/8/8 w - - "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "

/* ------------------------------- functions ------------------------------- */
// generate castling moves
void generate_castling_moves(board_state *board, move_list *moves, const int side, const U64 *bitboards,
                            const U64 *occupancies, const int castle);

// print state of board
void print_board(const board_state *board);

// initialize all tables
void init_all();

// initialize position from FEN string
void parse_fen(board_state *board, const char *fen);

// get whether square is currently attacked by given side
int square_attacked(board_state *board, const int square, const int side);

// get whether side has legal moves
int has_legal_moves(board_state *board);

// generate all moves
void generate_moves(board_state *board, move_list *moves);

// make move on chess board
int make_move(board_state *board, const int move, const int move_flag);

// position evaluation
int evaluate(const board_state *board);

/* ------------------------------- variables ------------------------------- */
// half move counter
// extern int ply;

// // bitboards
// extern U64 bitboards[12];

// // occupancy bitboards
// extern U64 occupancies[3];

// // side to move
// extern int side;

// // enpassant square
// extern int enpassant;

// // castling rights
// extern int castle;

#endif