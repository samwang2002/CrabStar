#include <iostream>
#include "board.hpp"
#include "bitboard.hpp"
#include "leapers.hpp"
#include "sliders.hpp"


int main()
{
    init_all();
    
    set_bit(bitboards[P], e2);
    print_bitboard(bitboards[P]);

    // std::cout << "piece: " << ascii_pieces[P] << "\n";
    // std::cout << ascii_pieces[char_pieces[K]] << std::endl;
    
    print_board();

    return 0;
}