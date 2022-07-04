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

#include "pawn.h"
#include "knight.h"
#include "king.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"

int main()
{
    init_attacks();

    parse_fen("8/p6P/8/8/8/8/8/8 w - - 0 1");
    print_board();
    printf("evaluation: %d cp\n", evaluate());

    // int debug = 1;
    // if (debug) {
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
    // } else
    //     uci_loop();

    return 0;
}