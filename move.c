#include <stdio.h>
#include "move.h"
#include "bitboard.h"

// add move to move list
void add_move(move_list *moves, int move)
{
    moves->moves[moves->count] = move;
    moves->count++;
}

// print description of move
void print_move(int move)
{
    #ifndef _WIN32
        printf("%s ", unicode_pieces[get_move_piece(move)]);
    #endif
    printf("%s ", square_to_coordinates[get_move_source(move)]);
    if (get_move_capture(move)) printf("x ");
    printf("%s", square_to_coordinates[get_move_target(move)]);
    if (get_move_promoted(move))
        #ifdef _WIN32
            printf("->%c", promoted_pieces[get_move_promoted(move)]);
        # else
            printf("->%s", unicode_pieces[get_move_promoted(move)]);
        #endif
    if (get_move_castling(move))
        printf("*");
}

//print move list
void print_move_list(move_list *moves)
{
    //do nothing on empty move list
    if(!moves->count)
    {
        printf("\n     No move in the move list!\n");
        return;
    }

    printf("\n     move    piece     capture   double    enpass    castling\n\n");

    //loop over moves within a move list
    for(int move_count = 0; move_count < moves->count; move_count++)
    {
        //init move
        int move = moves->moves[move_count];

        //print move
        printf("     %s", square_to_coordinates[get_move_source(move)]);
        printf("%s", square_to_coordinates[get_move_target(move)]);
        printf("%c   ", get_move_promoted(move) ? promoted_pieces[get_move_promoted(move)] : ' ');
        #ifdef WIN64
            printf("%c         ", ascii_pieces[get_move_piece(move)]);
        #else
            printf("%s         ", unicode_pieces[get_move_piece(move)]);
        #endif
        printf("%d         ", get_move_capture(move) ? 1 : 0);
        printf("%d         ", get_move_double(move) ? 1 : 0);
        printf("%d         ", get_move_enpassant(move) ? 1 : 0);
        printf("%d         \n", get_move_castling(move) ? 1 : 0);
    }
    //print total number of moves
    printf("\n\n     Total number of moves: %d\n\n", moves->count);
}