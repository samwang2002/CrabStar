#include <iostream>
#include "board.hpp"
#include "bitboard.hpp"
#include "leapers.hpp"
#include "sliders.hpp"


int main()
{
    init_all();
    parse_fen(tricky_position);
    print_board();
    std::cout << square_attacked(h2, black) << std::endl;
}