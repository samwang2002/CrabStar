#include <iostream>
#include "board.hpp"
#include "bitboard.hpp"
#include "move.hpp"
#include "leapers.hpp"
#include "sliders.hpp"


int main()
{
    init_all();
    // parse_fen(tricky_position);
    parse_fen("r3k2r/p11pqpb1/bn2pnp1/2pPN3/Pp2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ");
    // parse_fen("8/6P1/8/8/8/6pP/6P1/8 w - -");
    print_board();

    move_list moves;
    moves.count = 0;
    // add_move(&moves, encode_move(a5, a6, P, 0, 0, 0, 0, 0));
    generate_pawn_moves(&moves, black);
    std::cout << "move count: " << moves.count << std::endl;
    for (int i = 0; i < moves.count; ++i) {
        print_move(moves.moves[i]);
        std::cout << std::endl;
    }
}