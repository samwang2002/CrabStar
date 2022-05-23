// System Headers

#include <iostream>
#include "bitboard.h"
#include "pawn.h"

int main()
{
    init_leapers_attacks();

    for (int square = 0; square < 64; square++)
        print_bitboard(pawn_attacks[white][square]);
    return 0;
}