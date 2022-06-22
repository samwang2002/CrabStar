#include <iostream>
#include <string.h>
#include "board.hpp"
#include "move.hpp"
#include "bitboard.hpp"
#include "constants.hpp"

#include "pawn.hpp"
#include "knight.hpp"
#include "king.hpp"
#include "bishop.hpp"
#include "rook.hpp"
#include "queen.hpp"

// declaring global variables
U64 bitboards[12];
U64 occupancies[3];
int side = white;
int enpassant = no_sq;
int castle = 0;

// print current state of board
void print_board()
{
    std::cout << std::endl;
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            if (!file) std::cout << "  " << 8-rank << " ";      // print ranks

            int square = rank*8 + file;
            int piece = -1;

            // loop over piece bitboards to see if any piece is present
            for (int i = 0; i < 12; ++i)
                if (get_bit(bitboards[i], square))
                    piece = i;
            
            std::cout << " ";
            #ifdef WIN64
                std::cout << ((piece == -1) ? '.' : ascii_pieces[piece]);
            #else
                std::cout << ((piece == -1) ? "." : unicode_pieces[piece]);
            #endif
        }
        std::cout << std::endl;
    }
    std::cout << "\n     a b c d e f g h\n\n";

    // print out game state variables
    std::cout << "     Side:     " << (side ? "black" : "white") << std::endl;
    std::cout << "     Enpass:      " << ((enpassant != no_sq) ? square_to_coordinates[enpassant] : "no")
              << std::endl;
    std::cout << "     Castling:  "
              << ((castle & wk) ? 'K' : '-')
              << ((castle & wq) ? 'Q' : '-')
              << ((castle & bk) ? 'k' : '-')
              << ((castle & bq) ? 'q' : '-')
              << std::endl << std::endl;
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
                int piece = char_pieces.at(*fen);
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