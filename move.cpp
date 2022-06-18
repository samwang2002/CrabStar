#include "move.hpp"
#include "bitboard.hpp"
#include <iostream>

// add move to move list
void add_move(move_list *moves, int move)
{
    moves->moves[moves->count] = move;
    // std::cout << "success 1\n";
    moves->count++;
    // std::cout << "success 2\n";
}

// print description of move
void print_move(int move)
{
    std::cout << square_to_coordinates[get_move_source(move)];
    if (get_move_capture(move)) std::cout << 'x';
    std::cout << square_to_coordinates[get_move_target(move)];
    if (get_move_promoted(move))
        std::cout << promoted_pieces[get_move_promoted(move)];
}