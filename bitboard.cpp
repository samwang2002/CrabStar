// headers
#include <iostream>
#include "bitboard.hpp"


/**********************************\
====================================

        Bit manipulations

====================================
\**********************************/

// print bitboard
void print_bitboard(U64 bitboard)
{
    // loop over board ranks
    for (int rank = 0; rank < 8; rank++)
    {
        // loop over board files
        for (int file = 0; file < 8; file++)
        {
            // convert file & rank into square index
            int square = rank * 8 + file;

            // print ranks
            if (!file)
                std::cout << "  " << (8 - rank) << " ";

            // print bit state (either 1 or 0)
            std::cout << " " << (get_bit(bitboard, square) ? '1' : '.' );

        }

        // print new line every rank
        std::cout << "\n";
    }

    // print board files
    std::cout << "\n     a b c d e f g h\n\n";

    // print bitboard as unsigned decimal number
    std::cout << "     Bitboard: " << bitboard << "d\n" << std::endl;

}

// index: integer where the ith bit is set iff the ith LSB within the attack mask is occupied
// attack mask: bitboard of attack mask
// returns bitboard with the indexed squares occupied
U64 set_occupancy(int index, U64 attack_mask)
{
    U64 occupancy = 0ULL;
    int masked_bits = count_bits(attack_mask);

    for (int count = 0; count < masked_bits; ++count) {
        int square = get_ls1b_index(attack_mask);
        attack_mask &= attack_mask-1;
        if (index & (1 << count))       // make sure occupancy is on board
            occupancy |= (1ULL << square);
    }

    return occupancy;
}