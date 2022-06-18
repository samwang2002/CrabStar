#include <iostream>
#include "board.hpp"
#include "bitboard.hpp"
#include "move.hpp"
#include "leapers.hpp"
#include "sliders.hpp"


int main()
{
    init_all();
    parse_fen(start_position);
    // print_board();

    move_list moves;
    moves.count = 0;
    // add_move(&moves, encode_move(a5, a6, P, 0, 0, 0, 0, 0));
    generate_pawn_moves(&moves, white);
    std::cout << "move count: " << moves.count << std::endl;
    for (int i = 0; i < moves.count; ++i) {
        print_move(moves.moves[i]);
        std::cout << std::endl;
    }
}