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

    print_bitboard((U64)get_random_U32_number());
    print_bitboard((U64)get_random_U32_number() & 0xFFFF);
    print_bitboard(get_random_U64_number());
    print_bitboard(get_random_U64_number() & get_random_U64_number() & get_random_U64_number());

    return 0;
}