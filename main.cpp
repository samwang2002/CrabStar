#include <iostream>
#include "board.hpp"
#include "bitboard.hpp"
#include "leapers.hpp"
#include "sliders.hpp"


int main()
{
    init_all();
    U64 occupancy = 0ULL;
    set_bit(occupancy, f6);
    print_bitboard(get_queen_attacks(d4, occupancy));
}