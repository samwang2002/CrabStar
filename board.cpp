#include <iostream>
#include "board.hpp"
#include "bitboard.hpp"
#include "leapers.hpp"
#include "sliders.hpp"

// declaring global variables
U64 bitboards[12];
U64 occupancies[3];
int side = -1;
int enpassant = no_sq;
int castle;

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