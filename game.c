#include "game.h"
#include "search.h"
#include "board.h"
#include "constants.h"
#include "net.h"
#include <stdio.h>

// returns result of match between player1 (white) and player2 (black)
int match(const net_weights *player1, const net_weights *player2, const int depth)
{
    int move_count = 0;
    move_list moves;
    while (move_count++ < max_moves) {
        // white moves
        negamax(-infinity, infinity, depth, player1);
        make_move(pv_table[0][0], all_moves);
        printf("%d ", move_count);
        print_move(pv_table[0][0]);
        printf(" ");
        
        // check if black has moves
        generate_moves(&moves);
        if (moves.count == 0) return (square_attacked(ls1b(bitboards[k]), white) ? 1 : 0);

        // black moves
        negamax(-infinity, infinity, depth, player2);
        make_move(pv_table[0][0], all_moves);
        print_move(pv_table[0][0]);
        printf(" \n");

        if (move_count % 10 == 0) print_board();

        // check if white has moves
        generate_moves(&moves);
        if (moves.count == 0) return (square_attacked(ls1b(bitboards[K]), black) ? -1 : 0);
    }
    return 0;
}
