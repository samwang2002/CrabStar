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

//print move list
void print_move_list(move_list *moves)
{
    //do nothing on empty move list
    if(!moves->count)
    {
        std::cout << "\n     No move in the move list!" << std::endl;
        return;
    }

    std::cout<<"\n     move    piece     capture   double    enpass    castling\n" << std::endl;

    //loop over moves within a move list
    for(int move_count = 0; move_count < moves->count; move_count++)
    {
        //init move
        int move = moves->moves[move_count];

        #ifdef WIN64
            //print move
            std::cout << "     " << square_to_coordinates[get_move_source(move)];
            std::cout << square_to_coordinates[get_move_target(move)];
            std::cout << (get_move_promoted(move) ? promoted_pieces[get_move_promoted(move)] : ' ') << "   ";
            std::cout << (ascii_pieces[get_move_piece(move)]) << "         ";
            std::cout << (get_move_capture(move) ? 1 : 0) << "         ";
            std::cout << (get_move_double(move) ? 1 : 0) << "         ";
            std::cout << (get_move_enpassant(move) ? 1 : 0) << "         ";
            std::cout << (get_move_castling(move) ? 1 : 0) << std::endl;

        #else
            //print move
            std::cout << "     " << square_to_coordinates[get_move_source(move)];
            std::cout << square_to_coordinates[get_move_target(move)];
            std::cout << (get_move_promoted(move) ? promoted_pieces[get_move_promoted(move)] : ' ') << "   ";
            std::cout << unicode_pieces[get_move_piece(move)] << "         ";
            std::cout << (get_move_capture(move) ? 1 : 0) << "         ";
            std::cout << (get_move_double(move) ? 1 : 0) << "         ";
            std::cout << (get_move_enpassant(move) ? 1 : 0) << "         ";
            std::cout << (get_move_castling(move) ? 1 : 0) << std::endl;
        
        #endif
    }
    //print total number of moves
    std::cout << "\n\n     Total number of moves: " << moves->count << "\n" << std::endl;
}