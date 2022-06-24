#ifdef WIN64
    #include <windows.h>
#else
    #include <sys/time.h>
#endif

#include <stdio.h>
#include "perf.h"
#include "move.h"
#include "board.h"
#include "constants.h"

int get_time_ms()
{
    #ifdef WIN64
        return GetTickCount();
    #else
        struct timeval time_value;
        gettimeofday(&time_value, NULL);
        return time_value.tv_sec * 1000 + time_value.tv_usec / 1000;
    #endif
}

// leaf nodes (number of positions reached during the test of the move generator at a given depth)
long nodes = 0;
long captures = 0;
long castles = 0;
long promotions = 0;
long enpassants = 0;
long checks = 0;
long checkmates = 0;

// perft driver
void perft_driver(int depth, int last_move)
{
    if (depth == 0) {
        ++nodes;
        if (get_move_capture(last_move)) ++captures;
        if (get_move_castling(last_move)) ++castles;
        if (get_move_promoted(last_move)) ++promotions;
        if (get_move_enpassant(last_move)) ++enpassants;
        // checks and checkmates
        if (!make_move(last_move, all_moves)) {
            if (square_attacked(ls1b(bitboards[(side == white) ? K : k]), side^1)) {
                ++checks;
                move_list moves;
                moves.count = 0;
                generate_moves(&moves, side);
                // printf("move count: %d\n", moves.count);
                if (moves.count == 0) ++checkmates;
            }
        }
        return;
    }
    
    // create move list instance
    move_list moves[1];
    
    // generate moves
    generate_moves(moves, side);
    
    // loop over generated moves
    for (int move_count = 0; move_count < moves->count; move_count++)
    {   
        // preserve board state
        copy_board();
        
        // make move
        int current_move = moves->moves[move_count];
        if (!make_move(current_move, all_moves))
            // skip to the next move
            continue;
        
        // call perft driver recursively
        perft_driver(depth - 1, current_move);
        
        // take back
        take_back();
    }
}

// perft test
void perft_test(int depth)
{
    printf("\n     Performance test\n\n");
    
    // create move list instance
    move_list moves[1];

    // init start time
    long start = get_time_ms();

    // special case for depth=0
    if (depth <= 0) {
        nodes = 1;
        goto PRINT_RESULTS;
    }

    // generate moves
    generate_moves(moves, side);

    // loop over generated moves
    for (int move_count = 0; move_count < moves->count; move_count++)
    {   
        // preserve board state
        copy_board();
        
        // make move
        int current_move = moves->moves[move_count];
        if (!make_move(current_move, all_moves))
            // skip to the next move
            continue;
        
        // cummulative nodes
        long cummulative_nodes = nodes;
        
        // call perft driver recursively
        perft_driver(depth - 1, current_move);
        
        // old nodes
        long old_nodes = nodes - cummulative_nodes;
        
        // take back
        take_back();
        
        // print move
        printf("     move: %s%s%c  nodes: %'ld\n", square_to_coordinates[get_move_source(current_move)],
                square_to_coordinates[get_move_target(current_move)],
                get_move_promoted(current_move) ? promoted_pieces[get_move_promoted(current_move)] : ' ',
                old_nodes);
    }
    
    // print results
    PRINT_RESULTS:
    printf("\n     Depth:        %d\n", depth);
    printf("     Nodes:        %'ld\n", nodes);
    printf("     Captures:     %'ld\n", captures);
    printf("     Castles:      %'ld\n", castles);
    printf("     Promotions:   %'ld\n", promotions);
    printf("     Enpassants:   %'ld\n", enpassants);
    printf("     Checks:       %'ld\n", checks);
    printf("     Checkmates:   %'ld\n", checkmates);
    printf("     Time:         %ldms\n\n", get_time_ms() - start);
}
