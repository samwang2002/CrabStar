// headers
#include <stdio.h>
#include "bitboard.h"

// get index of least significant bit
int ls1b(const U64 bitboard)
{
    if (bitboard) return count_bits((bitboard & -bitboard)-1);
    return -1;
}

// print bitboard
void print_bitboard(const U64 bitboard)
{
    // loop over board ranks
    for (int rank = 0; rank < 8; ++rank)
    {
        // loop over board files
        for (int file = 0; file < 8; ++file)
        {
            // convert file & rank into square index
            int square = rank * 8 + file;

            // print ranks
            if (!file)
                printf(" %d ", 8-rank);

            // print bit state (either 1 or 0)
            printf(" %c", get_bit(bitboard, square) ? '1' : '.');

        }

        // print new line every rank
        printf("\n");
    }

    // print board files
    printf("\n     a b c d e f g h\n\n");

    // print bitboard as unsigned decimal number
    printf("     Bitboard: %llud\n\n", bitboard);

}

// index: integer where the ith bit is set iff the ith LSB within the attack mask is occupied
// attack mask: bitboard of attack mask
// returns bitboard with the indexed squares occupied
U64 set_occupancy(const int index, const int masked_bits, U64 attack_mask)
{
    U64 occupancy = 0ULL;

    for (int count = 0; count < masked_bits; ++count) {
        int square = ls1b(attack_mask);
        attack_mask &= attack_mask-1;
        if (index & (1 << count))       // make sure occupancy is on board
            occupancy |= (1ULL << square);
    }

    return occupancy;
}