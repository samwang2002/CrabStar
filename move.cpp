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
    #ifndef _WIN32
        std::cout << unicode_pieces[get_move_piece(move)] << ' ';
    #endif
    std::cout << square_to_coordinates[get_move_source(move)] << ' ';
    if (get_move_capture(move)) std::cout << "x ";
    std::cout << square_to_coordinates[get_move_target(move)];
    if (get_move_promoted(move))
        #ifdef _WIN32
            std::cout << "->" << promoted_pieces[get_move_promoted(move)];
        # else
            std::cout << "->" << unicode_pieces[get_move_promoted(move)];
        #endif
    if (get_move_castling(move))
        std::cout << '*';
}