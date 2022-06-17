// headers
#include <iostream>
#include "bitboard.hpp"

// declaring global variables
const char* square_to_coordinates[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};

// get index of least significant bit
inline int get_ls1b_index(U64 bitboard)
{
    if (bitboard) return count_bits((bitboard & -bitboard)-1);
    return -1;
}

// print bitboard
void print_bitboard(U64 bitboard)
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
U64 set_occupancy(int index, int masked_bits, U64 attack_mask)
{
    U64 occupancy = 0ULL;

    for (int count = 0; count < masked_bits; ++count) {
        int square = get_ls1b_index(attack_mask);
        attack_mask &= attack_mask-1;
        if (index & (1 << count))       // make sure occupancy is on board
            occupancy |= (1ULL << square);
    }

    return occupancy;
}