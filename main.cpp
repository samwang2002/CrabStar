#include <iostream>
#include <string.h>
#include "board.hpp"
#include "bitboard.hpp"
#include "move.hpp"
#include "leapers.hpp"
#include "sliders.hpp"


int main()
{
    init_all();
    // parse_fen(tricky_position);
    parse_fen("r3k2r/pP1pqpb1/bn2pnp1/2pPN3/1p2P3/2N2Q11/1PPPBPPP/R3K2R w KQkq - 0 1 ");
    // parse_fen("8/6P1/8/8/8/6pP/6P1/8 w - -");
    print_board();
    
    // //preserve board state
    // copy_board();

    // //prase fen
    // parse_fen(empty_board);
    // print_board();

    // // restore board state
    // take_back();

    // print_board();

    move_list moves;
    moves.count = 0;
    generate_queen_moves(&moves, black);
    print_move_list(&moves);

    return 0;
}