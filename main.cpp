#include <iostream>
#include "board.hpp"
#include "bitboard.hpp"
#include "leapers.hpp"
#include "sliders.hpp"


int main()
{
    init_all();
    
    set_bit(bitboards[P], a2);
    set_bit(bitboards[P], b2);
    set_bit(bitboards[P], c2);
    set_bit(bitboards[P], d2);
    set_bit(bitboards[P], e2);
    set_bit(bitboards[P], f2);
    set_bit(bitboards[P], g2);
    set_bit(bitboards[P], h2);

    set_bit(bitboards[N], b1);
    set_bit(bitboards[N], g1);
    
    print_board();

    return 0;
}