#include <iostream>
#include "bitboard.hpp"
#include "pawn.hpp"
#include "knight.hpp"
#include "king.hpp"
#include "bishop.hpp"
#include "rook.hpp"

// init leaper pieces attacks
void init_leapers_attacks()
{
    // loop over 64 board squres
    for (int square = 0; square < 64; square++)
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

int main()
{
    init_leapers_attacks();

    std::cout<< get_random_number() << "\n";
    std::cout<< get_random_number() << "\n";
    std::cout<< get_random_number() << "\n";
    std::cout<< get_random_number() << "\n";
    std::cout<< get_random_number() << "\n";

    return 0;
}