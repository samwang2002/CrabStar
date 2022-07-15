#include "game.h"
#include "search.h"
#include "board.h"
#include "constants.h"
#include "net.h"
#include <stdio.h>

// returns result of match between player1 (white) and player2 (black)
int match(const net_weights *player1, const net_weights *player2, const int depth, const int verbose)
{
    int move_count = 0;
    
    while (move_count++ < max_moves) {
        // white moves
        negamax(-infinity, infinity, depth, player1);
        make_move(pv_table[0][0], all_moves);

        if (verbose) {
            printf("%d ", move_count);
            print_move(pv_table[0][0]);
            printf(" ");
        }

        if (!has_legal_moves()) return (square_attacked(ls1b(bitboards[k]), white) ? 1 : 0);

        // black moves
        negamax(-infinity, infinity, depth, player2);
        make_move(pv_table[0][0], all_moves);

        if (verbose) {
            print_move(pv_table[0][0]);
            printf("\n");
        }

        if (!has_legal_moves()) return (square_attacked(ls1b(bitboards[K]), black) ? -1 : 0);
    }
    return 0;
}
