#include <iostream>
#include "board.hpp"
#include "bitboard.hpp"
#include "leapers.hpp"
#include "sliders.hpp"


int main()
{
    init_all();
    parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq a1 0 1 ");
    // parse_fen(tricky_position);
    print_board();
    print_bitboard(occupancies[both]);
}