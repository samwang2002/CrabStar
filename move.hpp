#ifndef MOVE_H
#define MOVE_H

// move list structure
typedef struct {
    int moves[256];
    int count;
} move_list;

// promoted pieces
static const char promoted_pieces[12] = {'p', 'n', 'b', 'r', 'q', 'k', 'p', 'n', 'b', 'r', 'q', 'k'};

// ASCII pieces
static const char ascii_pieces[13] = "PNBRQKpnbrqk";

// unicode pieces
static const char *unicode_pieces[12] = {"♙", "♘", "♗", "♖", "♕", "♔", "♟", "♞", "♝", "♜", "♛", "♚"};

/* ------------------------------- macros ------------------------------- */
// encodes all data about a move in an integer
// castling (1b), enpassant (1b), double push (1b), capture (1b), promoted piece (4b),
// piece (4b), target square (6b), source (6b), 24b total
#define encode_move(source, target, piece, promoted, capture, double_step, enpassant, castling) \
    ((source) |             \
    ((target) << 6) |       \
    ((piece) << 12) |       \
    ((promoted) << 16) |    \
    ((capture) << 20) |     \
    ((double_step) << 21) | \
    ((enpassant) << 22) |   \
    ((castling) << 23))     \

// extract data from moves
#define get_move_source(move) ((move) & 0x3f)
#define get_move_target(move) (((move) & 0xfc0) >> 6)
#define get_move_piece(move) (((move) & 0xf000) >> 12)
#define get_move_promoted(move) (((move) & 0xf0000) >> 16)
#define get_move_capture(move) ((move) & 0x100000)
#define get_move_double(move) ((move) & 0x200000)
#define get_move_enpassant(move) ((move) & 0x400000)
#define get_move_castling(move) ((move) & 0x800000)

/* ------------------------------- functions ------------------------------- */
// add move to move list
void add_move(move_list *moves, int move);

// print description of move
void print_move(int move);

void print_move_list(move_list *moves);

#endif