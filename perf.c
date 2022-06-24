#ifdef WIN64
    #include <windows.h>
#else
    #include <sys/time.h>
#endif

#include <stdio.h>
#include "perf.h"
#include "move.h"
#include "board.h"

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

// perft driver
void perft_driver(int depth)
{
    // reccursion escape condition
    if (depth == 0)
    {
        // increment nodes count (count reached positions)
        nodes++;
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
        if (!make_move(moves->moves[move_count], all_moves))
            // skip to the next move
            continue;
        
        // call perft driver recursively
        perft_driver(depth - 1);
        
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

    // generate moves
    generate_moves(moves, side);

    // init start time
    long start = get_time_ms();

    // loop over generated moves
    for (int move_count = 0; move_count < moves->count; move_count++)
    {   
        // preserve board state
        copy_board();
        
        // make move
        if (!make_move(moves->moves[move_count], all_moves))
            // skip to the next move
            continue;
        
        // cummulative nodes
        long cummulative_nodes = nodes;
        
        // call perft driver recursively
        perft_driver(depth - 1);
        
        // old nodes
        long old_nodes = nodes - cummulative_nodes;
        
        // take back
        take_back();
        
        // print move
        printf("     move: %s%s%c  nodes: %ld\n", square_to_coordinates[get_move_source(moves->moves[move_count])],
                                                 square_to_coordinates[get_move_target(moves->moves[move_count])],
                                                 get_move_promoted(moves->moves[move_count]) ? promoted_pieces[get_move_promoted(moves->moves[move_count])] : ' ',
                                                 old_nodes);
    }
    
    // print results
    printf("\n     Depth: %d\n", depth);
    printf("     Nodes: %ld\n", nodes);
    printf("     Time: %ld\n\n", get_time_ms() - start);
}
