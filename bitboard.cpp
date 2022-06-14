// headers
#include <iostream>
#include "bitboard.h"


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