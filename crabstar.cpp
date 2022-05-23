// System Headers

#include <iostream>
#include "bitboard.h"


int main()
{
    //define bitboard
    U64 bitboard = 0ULL;

    // setting some bits
    set_bit(bitboard, e4);
    set_bit(bitboard, c3);
    set_bit(bitboard, f2);

    //print bitboard
    print_bitboard(bitboard);

    //reset bit
    pop_bit(bitboard, e4);

    //print bitboard
    print_bitboard(bitboard);
    
    return 0;
}