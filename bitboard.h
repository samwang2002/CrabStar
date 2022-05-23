// system headers
#include <iostream>

//define bitboard data type
#define U64 unsigned long long

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

            //
            std::cout << " " << square << " ";

        }

        // print new line every rank
        std::cout << "\n";
    }
}