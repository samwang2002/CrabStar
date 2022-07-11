#ifndef BOARD_H
#define BOARD_H

#include "bitboard.h"
#include "move.h"
#include <string.h>

#define max_ply 64
#define input_nodes 1025        // last char is newline

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

// initialize all tables
void init_all();

// initialize position from FEN string
void parse_fen(const char *fen);

// get whether square is currently attacked by given side
int square_attacked(const int square, const int side);

// get whether current side is in check
int in_check();

// get whether side has legal moves
int has_legal_moves();

// generate all moves
void generate_moves(move_list *moves);

// make move on chess board
int make_move(const int move, const int move_flag);

// score move - determines priority of move
int score_move(const int move);

// sort moves in descending order by score
void sort_moves(move_list *moves);

// position evaluation
int evaluate();

// negamax alpha beta search
int negamax(const int alpha, const int beta, int depth);

// quiescence search: similar to negamax but only examine captures
int quiescence(const int alpha, const int beta);

// search position for best move and print it to UCI interface
void search_position(const int depth);

// enable PV mopve scoring
void enable_pv_scoring(move_list *moves);

// encode position for neural network
char *encode_position();

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

// killer moves [id][ply]
// can use different number of killer moves but 2 is a good choice
// see: https://www.chessprogramming.org/Killer_Heuristic
extern int killer_moves[2][max_ply];

// history moves [piece][square]
// prioritizes moves that cause more cutoffs in search tree
// see: https://www.chessprogramming.org/History_Heuristic
extern int history_moves[12][64];

// PV (principal variation) length [ply]
extern int pv_length[max_ply];

// PV table
// see: https://www.reddit.com/r/chessprogramming/comments/m2m048/how_does_a_triangular_pvtable_work/
extern int pv_table[max_ply][max_ply];
/*
    diagonal structure:
    m1 m2 m3  ... <- principal variation
    0  m2 m3  ...
    0  0  m3  ...
    .  .  .
*/

// follow PV flag
extern int follow_pv;

// score PV PV flag
extern int score_pv;

//late move reduction variables
extern int full_depth_moves;
extern int reduction_limit;

#endif