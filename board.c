#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "board.h"
#include "move.h"
#include "bitboard.h"
#include "constants.h"

#include "pawn.h"
#include "knight.h"
#include "king.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"

// declaring global variables
U64 bitboards[12];
U64 occupancies[3];
int side = white;
int enpassant = no_sq;
int castle = 0;

// print current state of board
void print_board()
{
    printf("\n");
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            if (!file) printf("  %d ", 8-rank);             // print ranks

            int square = rank*8 + file;
            int piece = -1;

            // loop over piece bitboards to see if any piece is present
            for (int i = 0; i < 12; ++i)
                if (get_bit(bitboards[i], square))
                    piece = i;
            
            #ifdef WIN64
                printf(" %c", (piece == -1) ? '.' : ascii_pieces[piece]);
            #else
                printf(" %s", (piece == -1) ? "." : unicode_pieces[piece]);
            #endif
        }
        printf("\n");
    }
    printf("\n     a b c d e f g h\n\n");

    // print out game state variables
    printf("     Side:     %s\n", side ? "black" : "white");
    printf("     Enpass:      %s\n", (enpassant != no_sq) ? square_to_coordinates[enpassant] : "no");
    printf("     Castling:  %c%c%c%c\n", (castle & wk) ? 'K' : '-', (castle & wq) ? 'Q' : '-',
           (castle & bk) ? 'k' : '-', (castle & bq) ? 'q' : '-');
}

// initialize all attacks
void init_attacks()
{
    init_pawn_attacks();
    init_knight_attacks();
    init_king_attacks();
    init_bishop_attacks();
    init_rook_attacks();
}

// parse fen from string
void parse_fen(const char *fen)
{
    // reset board position and state variables
    memset(bitboards, 0, sizeof(bitboards));
    memset(occupancies, 0, sizeof(occupancies));
    side = 0;
    enpassant = no_sq;
    castle = 0;

    // load position
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            int square = rank*8 + file;
            if (isalpha(*fen)) {            // piece
                int piece = char_pieces[(int)*fen];
                set_bit(bitboards[piece], square);
            } else if (isdigit(*fen)) {     // empty squares
                int offset = *fen - '0';
                file += offset-1;       // subtract one because file automatically increments
            }
            ++fen;
        }
        ++fen;
    }

    // get game state variables
    // side
    (*fen == 'w') ? (side=white) : (side=black);
    fen += 2;

    // castling
    while (*fen != ' ') {
        switch (*fen) {
            case 'K': castle |= wk; break;
            case 'Q': castle |= wq; break;
            case 'k': castle |= bk; break;
            case 'q': castle |= bq; break;
        }
        ++fen;
    }
    ++fen;

    // en passant square
    if (*fen != '-') {
        int file = fen[0] - 'a';
        int rank = 8 - (fen[1] - '0');
        enpassant = rank*8 + file;
    } else enpassant = no_sq;

    // initialize occupancies
    for (int piece = P; piece <= K; ++piece) occupancies[white] |= bitboards[piece];
    for (int piece = p; piece <= k; ++piece) occupancies[black] |= bitboards[piece];
    occupancies[both] |= occupancies[white] | occupancies[black];
}

// get whether square is currently attacked by given side
int square_attacked(int square, int side)
{
    if (side == white) {
        U64 attacks = (pawn_attacks[black][square] & bitboards[P])              // not a typo
                    | (knight_attacks[square] & bitboards[N])
                    | (get_bishop_attacks(square, occupancies[both]) & bitboards[B])
                    | (get_rook_attacks(square, occupancies[both]) & bitboards[R])
                    | (get_queen_attacks(square, occupancies[both]) & bitboards[Q])
                    | (king_attacks[square] & bitboards[K]);
        return attacks ? 1 : 0;
    } else {
        U64 attacks = (pawn_attacks[white][square] & bitboards[p])
                    | (knight_attacks[square] & bitboards[n])
                    | (get_bishop_attacks(square, occupancies[both]) & bitboards[b])
                    | (get_rook_attacks(square, occupancies[both]) & bitboards[r])
                    | (get_queen_attacks(square, occupancies[both]) & bitboards[q])
                    | (king_attacks[square] & bitboards[k]);
        return attacks ? 1 : 0;
    }
}

// generate all moves
void generate_moves(move_list *moves, int side)
{
    generate_pawn_moves(moves, side, bitboards, occupancies, enpassant);
    generate_knight_moves(moves, side, bitboards, occupancies);
    generate_castling_moves(moves, side, bitboards, occupancies, castle, &square_attacked);
    generate_king_moves(moves, side, bitboards, occupancies);
    generate_bishop_moves(moves, side, bitboards, occupancies);
    generate_rook_moves(moves, side, bitboards, occupancies);
    generate_queen_moves(moves, side, bitboards, occupancies);
}

// make move on chess board
// move_flag is either all_moves or captures_only
// if move_flag == captures_only, make_move will only update the board if the given move is a capture
int make_move(int move, int move_flag)
{
    if (move_flag == all_moves) {           // make move
        // preserve board state
        copy_board();

        // parse move
        int source = get_move_source(move);
        int target = get_move_target(move);
        int piece = get_move_piece(move);
        int promoted_piece = get_move_promoted(move);
        // int capture = get_move_capture(move);
        // int double_step = get_move_double(move);
        int enpass = get_move_enpassant(move);
        // int castling = get_move_castling(move);

        // clear source square
        pop_bit(bitboards[piece], source);
        pop_bit(occupancies[side], target);

        // clear target square
        if (side == white) {
            for (int i = p; i <= k; ++i)
                pop_bit(bitboards[i], target);
            pop_bit(occupancies[black], target);
        } else {
            for (int i = P; i <= K; ++i)
                pop_bit(bitboards[i], target);
            pop_bit(occupancies[white], target);
        }

        // place piece on target square
        if (promoted_piece)
            set_bit(bitboards[promoted_piece], target);
        else
            set_bit(bitboards[piece], target);
        set_bit(occupancies[side], target);
        set_bit(occupancies[both], target);

        // handle enpassant captures
        if (enpass) {
            if (side == white) {
                pop_bit(bitboards[p], target+8);
                pop_bit(occupancies[black], target+8);
                pop_bit(occupancies[both], target+8);
            } else {
                pop_bit(bitboards[P], target-8);
                pop_bit(occupancies[white], target-8);
                pop_bit(occupancies[both], target-8);
            }
        }
        enpassant = no_sq;

        return 1;
    } else {                                // only make capture moves
        if (get_move_capture(move)) make_move(move, all_moves);
        else return 0;
    }
}