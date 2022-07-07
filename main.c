#include <stdio.h>
#include <string.h>
#include <locale.h>

#include "board.h"
#include "uci.h"
#include "bitboard.h"
#include "move.h"
#include "constants.h"
#include "perft.h"
#include "timecontrol.h"
#include "hash.h"

#include "pawn.h"
#include "knight.h"
#include "king.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"

int main()
{
    init_all();

    // parse_fen("6k1/p5p1/8/8/8/8/6PP/6K1 w - - 0 1");
    // print_board();
    // printf("evaluation: %d cp\n", evaluate());

    int debug = 1;
    if (debug) {
        parse_fen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ");
        print_board();
        perft_test(2);
        //printf("hash key: %llx\n", generate_hash_key());
    //     parse_fen("8/5pk1/3Rp1p1/2p1P1Pp/2P1P2P/R7/3r4/5bK1 b - - 0 38");
    //     print_board();
    //     search_position(7);

    //     // move_list moves;
    //     // generate_moves(&moves);
    //     // killer_moves[0][ply] = moves.moves[0];
    //     // killer_moves[1][ply] = moves.moves[2];
    //     // history_moves[P][a4] = 30;
    //     // sort_moves(&moves);

    //     // for (int i = 0; i < moves.count; ++i) {
    //     //     print_move(moves.moves[i]);
    //     //     printf(": %d\n", score_move(moves.moves[i]));
    //     // }
    } else
         uci_loop();
    return 0;
}