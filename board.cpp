#include <iostream>
#include "board.hpp"
#include "bitboard.hpp"
#include "leapers.hpp"
#include "sliders.hpp"

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

// initialize attack tables for leapers
void init_leapers_attacks()
{
    // loop over 64 board squres
    for (int square = 0; square < 64; ++square)
    {
        // inititialize pawn attacks
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);

        // init knight attacks
        knight_attacks[square] = mask_knight_attacks(square);

        // init king attacks
        king_attacks[square] = mask_king_attacks(square);
    }
}

// initialize all attacks
void init_all()
{
    init_leapers_attacks();
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
    for (int piece = P; piece <= K; ++piece)        // white pieces
        occupancies[white] |= bitboards[piece];
    for (int piece = p; piece <= k; ++piece)        // black pieces
        occupancies[black] |= bitboards[piece];
    occupancies[both] |= occupancies[white] | occupancies[black];
}