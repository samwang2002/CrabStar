// System Headers

#include <iostream>
#include "bitboard.h"
#include "pawn.h"
#include "knight.h"

// init leaper pieces attacks
void init_leapers_attacks()
{
    // loop over 64 board squres
    for (int square = 0; square < 64; square++)
    {
        // inititialize pawn attacks
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);

        //init knight attacks
        knight_attacks[square] = mask_knight_attacks(square);
    }

}

int main()
{
    init_leapers_attacks();

    for (int square = 0; square < 64; square++)
        print_bitboard(knight_attacks[square]);

    return 0;
}